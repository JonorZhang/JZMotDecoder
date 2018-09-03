//
//  JZMotDecoder.m
//  mot
//
//  Created by Jonor on 16/7/5.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#import "JZMotDecoder.h"
#import "JZMotDataGroup.h"
#import "JZMotDirectory.h"
#import "JZMotSegment.h"
#import "JZMotObject.h"
#import "JZMotObjectHeader.h"
#import "JZMotObjectBody.h"
#import "JZMotHeaderParameterVersion.h"

@interface JZMotDecoder ()
@property (nonatomic, strong) JZMotDirectory *nextMotDirectory;
@property (nonatomic, strong) JZMotDirectory *currentMotDirectory;

@property (nonatomic, assign) ushort currentTansportID;

@end


@implementation JZMotDecoder

+ (instancetype)decoder {
    JZMotDecoder *motDecoder = [[JZMotDecoder alloc] init];
    motDecoder.motObjectNameLookup = [NSMutableDictionary dictionary];
    motDecoder.motObjectsTransportIdLookup = [NSMutableDictionary dictionary];
//    motDecoder.nextMotDirectory = [JZMotDirectory directory];
//    motDecoder.currentMotDirectory = [JZMotDirectory directory];
    
    return motDecoder;
}

//public IList<string> GetObjectNames()
//{
//    if (currentMOTDirectory != null)
//    {
//        var names = new List<string>();
//        
//        // Loop through the object headers from the MOT directory.
//        foreach (var objectTransportId in currentMOTDirectory.GetMOTObjectTransportIds())
//        {
//            if (!motObjectsTransportIdLookup.ContainsKey(objectTransportId))
//                continue;
//            
//            var header = currentMOTDirectory.GetMOTObjectHeader(objectTransportId);
//            var motObject = motObjectsTransportIdLookup[objectTransportId];
//            
//            if (motObject.IsObjectComplete && !names.Contains(header.ContentName))
//                names.Add(header.ContentName);
//        }
//        
//        return names;
//    }
//    
//    return (from key in motObjectNameLookup.Keys orderby key select key).ToArray();
//}
//
//public IList<ushort> GetTransportIds()
//{
//    return motObjectsTransportIdLookup.Keys.ToList();
//}
//
//public MOTObject GetObject(string objectName)
//{
//    if (currentMOTDirectory != null)
//    {
//        // Loop through the object headers from the MOT directory.
//        MOTObject motObject = null;
//        foreach (var transportId in currentMOTDirectory.GetMOTObjectTransportIds())
//        if (motObjectsTransportIdLookup.ContainsKey(transportId))
//        {
//            var header = currentMOTDirectory.GetMOTObjectHeader(transportId);
//            motObject = motObjectsTransportIdLookup[transportId];
//            
//            if (header.ContentName == objectName && motObject.IsObjectComplete)
//                return motObject;
//        }
//        
//        return motObject;
//    }
//    
//    return motObjectNameLookup.ContainsKey(objectName) ? motObjectNameLookup[objectName] : null;
//}
//
//public MOTObject GetObject(ushort transportId)
//{
//    if (motObjectsTransportIdLookup.ContainsKey(transportId))
//        return motObjectsTransportIdLookup[transportId];
//    
//    return null;
//}

- (JZMotObject *)ProcessDataGroups:(NSArray<NSData *> *)dataGroups CheckCRC:(BOOL)crc
{
    JZMotObject *motObject;
    
    if (crc == NO) {
        NSLog(@"不校验CRC");
    }
    
    for (NSData *dataGroup in dataGroups) {
        [self ProcessDataGroup:dataGroup CheckCRC:crc];
    }
    motObject = [self.motObjectsTransportIdLookup valueForKey:[NSString stringWithFormat:@"%d", self.currentTansportID]];
    
    return motObject;
}


// 流程：4 将一个groupData(含0x73/0x74)也就是dataBuffer 进行处理
- (void)ProcessDataGroup:(NSData *)groupData CheckCRC:(BOOL)crc
{
    JZMotDataGroup *group = [JZMotDataGroup dataGroup:(Byte *)groupData.bytes length:groupData.length checkCRC:crc];
    if (group == nil ) return;
    JZMotSegment *segment = [JZMotSegment segment:group];
    if (segment == nil) return;

    if (group.DataGroupType != DataGroupTypesMotDirectory)
    {
        JZMotObject *motObject ;
        NSString *key = [NSString stringWithFormat:@"%d", group.TransportId];
        if ((motObject = [self.motObjectsTransportIdLookup valueForKey:key]) == nil) {
            motObject = [JZMotObject objectWithTransportId:group.TransportId];
            [self.motObjectsTransportIdLookup addEntriesFromDictionary:[NSDictionary dictionaryWithObject:motObject forKey:key]];
        }
        [motObject addSegment:segment];
        self.currentTansportID = group.TransportId;
        
        // If the object is complete then add it to the objects that are complete
        // and have a name.
        if (self.currentMotDirectory == nil && [motObject isObjectComplete] && motObject.motObjectHeader.ContentName != nil)
        {
            // For now just add the item to the list but you'll have to process updates differently.
            if ([self.motObjectNameLookup valueForKey:motObject.motObjectHeader.ContentName])
            {
                // Get the old MOT object that might need to be updated.
                JZMotObject *oldMotObject = [self.motObjectNameLookup valueForKey:motObject.motObjectHeader.ContentName];
                
                // If this already is registered do not add it.
                if (oldMotObject == motObject || oldMotObject.transportId == motObject.transportId)
                    return;
                
                // This is probably a new object or a header update.
                if (motObject.motObjectHeader.ContentType == MOTContentTypeMotTransport)
                {
                    // See if the other criteria is necessary for updating the object.
                    if (motObject.motObjectHeader.ContentSubType == 0 || motObject.motObjectHeader.BodySize == 0)
                    {
                        // Check to see if the version parameter exists.
                        JZMotHeaderParameterVersion *parameter = (JZMotHeaderParameterVersion *)[motObject.motObjectHeader getParameter:MOTParameterTypeVersionNumber];
                        
                        // If the version number matches or it doesn't exist then update the item.
                        if (parameter == nil || parameter.version == oldMotObject.motObjectHeader.Version)
                        {
                            // Get all the parameters from the new object sent and add it to the old object.
                            [oldMotObject.motObjectHeader UpdateParameters:[motObject.motObjectHeader getParameters]];
// 什么鬼
//                            // If one of the parameters is the trigger parameter see if it is now.  If so then call the event.
//                            var triggerParameter = (MOTHeaderParameterTime)motObject.MOTObjectHeader.GetParameter(MOTParameterType.TriggerTime);
//                            
//                            if (triggerParameter != null && triggerParameter.IsNow && ObjectTriggered != null)
//                                ObjectTriggered(this, oldMotObject);
                        }
                    }
                }
                else
                {
                    // The object probably needs to be replaced.  So replace the object with the new object.
                    // Replace in the name lookup.
                    [self.motObjectNameLookup removeObjectForKey:motObject.motObjectHeader.ContentName];
                    [self.motObjectNameLookup addEntriesFromDictionary:[NSDictionary dictionaryWithObject:motObject forKey:motObject.motObjectHeader.ContentName]];

                    // Remove the old object from the dictionary.
                    [self.motObjectsTransportIdLookup removeObjectForKey:[NSString stringWithFormat:@"%d", oldMotObject.transportId]];
                    // If one of the parameters is the trigger parameter see if it is now.  If so then call the event.
//  暂时没处理
//                    var triggerParameter = (MOTHeaderParameterTime)motObject.MOTObjectHeader.GetParameter(MOTParameterType.TriggerTime);
//                    
//                    if (triggerParameter != null && triggerParameter.IsNow && ObjectTriggered != null)
//                        ObjectTriggered(this, motObject);
                }
            }
            else
            {
                [self.motObjectNameLookup addEntriesFromDictionary:[NSDictionary dictionaryWithObject:motObject forKey:motObject.motObjectHeader.ContentName]];
//  暂时没处理
//                // If one of the parameters is the trigger parameter see if it is now.  If so then call the event.
//                var triggerParameter = (MOTHeaderParameterTime)motObject.MOTObjectHeader.GetParameter(MOTParameterType.TriggerTime);
//                
//                if (triggerParameter != null && triggerParameter.IsNow && ObjectTriggered != null)
//                    ObjectTriggered(this, motObject);
            }
        }
        else if (self.currentMotDirectory != nil)
        {
            // If one of the parameters is the trigger parameter see if it is now.  If so then call the event.
//  暂时没处理
//            var triggerParameter = (MOTHeaderParameterTime)motObject.MOTObjectHeader.GetParameter(MOTParameterType.TriggerTime);
//            
//            if (triggerParameter != null && triggerParameter.IsNow && ObjectTriggered != null)
//                ObjectTriggered(this, motObject);
        }
    } else {
//        // Determine if the packet received even needs to be handled.
//        if (currentMOTDirectory == null || currentMOTDirectory.TransportId != group.TransportId)
//        {
//            // Since the current MOT directory is not the same as this directory build up the next
//            // directory.
//            if (nextMotDirectory == null || nextMotDirectory.TransportId != group.TransportId)
//                // Create a new MOT directory object.
//                nextMotDirectory = new MOTDirectory(group.TransportId);
//            
//            // Now add the segment to the MOT directory
//            nextMotDirectory.AddMOTSegment(segment);
//            
//            // Now if the next mot directory is complete it should become the current directory.
//            if (nextMotDirectory.DirectoryComplete)
//            {
//                // Get the new transport Ids
//                var newTransportIds = new List<ushort>(nextMotDirectory.GetMOTObjectTransportIds());
//                
//                var firstDirectory = false;
//                if (currentMOTDirectory != null)
//                {
//                    // Figure out what transport ID's were in the old directory and are no longer needed.
//                    var oldTransportIds = new List<ushort>(currentMOTDirectory.GetMOTObjectTransportIds());
//                    
//                    // Loop through the new Transport ID's and if they exist in the old remove them from the new and the old.
//                    for (var i = (newTransportIds.Count - 1); i >= 0; i--)
//                        // If the element exists in the old TransportID's remove them from both arrays.
//                        if (oldTransportIds.Contains(newTransportIds[i]))
//                        {
//                            // Since this object already existed we do not need to add an object for it.
//                            oldTransportIds.Remove(newTransportIds[i]);
//                            newTransportIds.RemoveAt(i);
//                        }
//                    
//                    // If there are any transport IDs left in the old transport Ids then they are not needed for the new directory
//                    // so remove them.
//                    foreach (var transportId in oldTransportIds)
//                    motObjectsTransportIdLookup.Remove(transportId);
//                }
//                else
//                    firstDirectory = true;
//                
//                // If there are any transport ID's that are not yet built that are in the new MOT directory then add them to the hash
//                foreach (var transportId in newTransportIds)
//                {
//                    MOTObject motObject;
//                    if (motObjectsTransportIdLookup.ContainsKey(transportId))
//                        motObject = motObjectsTransportIdLookup[transportId];
//                    else
//                    {
//                        motObject = new MOTObject(transportId);
//                        motObjectsTransportIdLookup.Add(transportId, motObject);
//                    }
//                    
//                    motObject.MOTObjectHeader = nextMotDirectory.GetMOTObjectHeader(transportId);
//                    motObject.MOTObjectBody.ProcessSegments(motObject.MOTObjectHeader.BodySize);
//                }
//                
//                // Copy the next MOT directory to the current MOT directory
//                currentMOTDirectory = nextMotDirectory;
//                
//                // Clear out the next MOT directory.
//                nextMotDirectory = null;
//                
//                // If this is the first directory then some management needs to occur.
//                if (firstDirectory)
//                {
//                    // Loop through all the objects in the name lookup.  That will no longer be used since we have a directory.
//                    foreach (var motObject in motObjectNameLookup.Values)
//                    // See if the object is in the directory.
//                    if (currentMOTDirectory.ContainsObject(motObject.TransportId))
//                        // Add the object back to the transportID lookup.
//                        if (motObjectsTransportIdLookup.ContainsKey(motObject.TransportId))
//                            motObjectsTransportIdLookup[motObject.TransportId] = motObject;
//                        else
//                            motObjectsTransportIdLookup.Add(motObject.TransportId, motObject);
//                    
//                    // Clear out the motObjectNameLookup hash.  Note this is not used for directory type mot transmissions.
//                    motObjectNameLookup.Clear();
//                }
//            }
//        }
    }
}



@end
