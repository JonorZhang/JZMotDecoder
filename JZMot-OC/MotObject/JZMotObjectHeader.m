//
//  JZMotObjectHeader.m
//  mot
//
//  Created by Jonor on 16/7/6.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#import "JZMotObjectHeader.h"
#import "JZMotSegment.h"
#import "JZMotHeaderParameter.h"
#import "JZMotHeaderParameterVersion.h"
#import "JZMotHeaderParameterText.h"

@interface JZMotObjectHeader ()

// <ushort, MOTSegment>
@property (nonatomic, strong) NSMutableDictionary *segments;
// <MOTParameterType, MOTHeaderParameter>
@property (nonatomic, strong) NSMutableDictionary * dictionaryMOTHeaderParameters;

@end


@implementation JZMotObjectHeader

+ (instancetype)header {
    JZMotObjectHeader *header = [[JZMotObjectHeader alloc] init];
    header.segments = [NSMutableDictionary dictionary];
    header.dictionaryMOTHeaderParameters = [NSMutableDictionary dictionary];
    header.ContentType = MOTContentTypeProprietaryTable;
    header.ContentName = [NSString string];
    header.MIMEType = [NSString string];
    header.MIMESubType = [NSString string];
    
    return header;
}

- (void)addSegment:(JZMotSegment *)segment
{
    // Determine if the segment has been already received.
    // If it has not already been received then add it to the hash.
//    if (self.segments.ContainsKey(segment.MSCDataGroup.MOTSegmentNumber))
//        return;
    NSString *key = [NSString stringWithFormat:@"%d", segment.dataGroup.MOTSegmentNumber];
    if ([self.segments valueForKey:key] != nil) {
        return;
    } else {
        [self.segments addEntriesFromDictionary:[NSDictionary dictionaryWithObject:segment forKey:key]];
    }
    
    // See if we can add info to the class based on this segment being added.
    // If this is the first segment then process it as it should be pretty easy.
    if (!self.isHeaderComplete)
        [self processSegments];
}

- (JZMotHeaderParameter *)getParameter:(MOTParameterType)parameterType
{
    JZMotHeaderParameter *parameter = [self.dictionaryMOTHeaderParameters valueForKey:[NSString stringWithFormat:@"%d", parameterType]];
    return parameter;
}

- (NSArray<JZMotHeaderParameter *> *)getParameters
{
    NSArray *parameters = [self.dictionaryMOTHeaderParameters allValues];
    return parameters;
}


- (void)UpdateParameters:(NSArray<JZMotHeaderParameter *> *)motHeaderParameters
{
    // The version and content name parameters should not be replaced.
    for (JZMotHeaderParameter *parameter in motHeaderParameters) {
        if ((MOTParameterType)parameter.ParameterId != MOTParameterTypeContentName && (MOTParameterType)parameter.ParameterId != MOTParameterTypeVersionNumber) {
            NSString *key = [NSString stringWithFormat:@"%d", parameter.ParameterId];
            if ([self.dictionaryMOTHeaderParameters valueForKey:key]) {
                [self.dictionaryMOTHeaderParameters setValue:parameter forKey:key];
            } else {
                [self.dictionaryMOTHeaderParameters addEntriesFromDictionary:[NSDictionary dictionaryWithObject:parameter forKey:key]];
            }
        }
    }
}

- (void)processSegments
{
    // Loop through the available segments and process them as they get populated
    NSMutableData *headerBytes = [NSMutableData data];
    JZMotSegment *segment;
    
    ushort segmentNumber = 0;
    while ((segment = [self.segments valueForKey:[NSString stringWithFormat:@"%d", segmentNumber]]) != nil)
    {
        // Add the bytes to the header bytes array list
        [headerBytes appendBytes:segment.segmentBytesData.bytes length:segment.segmentBytesData.length];
        
        // Go to the next segment
        segmentNumber++;
    }
    
    // Now see if we have enough bytes in the array to get the header size.
    if (headerBytes.length < 6)
        return;
    
    Byte *bytes = (Byte *)headerBytes.bytes;
    ushort headerSize = (ushort)(((bytes[3] & 0x0F) << 9) | (bytes[4] << 1) | (bytes[5] >> 7));
    
    // Now if there is enough bytes total to process the header then do it.
    if (headerSize > headerBytes.length)
        return;
    
    // There is enough bytes to process the header so do it.
    self.BodySize = (uint)((bytes[0] << 20) | (bytes[1] << 12) | (bytes[2] << 4) | (bytes[3] >> 4));
    self.ContentType = (MOTContentType)((bytes[5] >> 1) & 0x3F);
    self.ContentSubType = (ushort)(((bytes[5] & 0x01) << 8) | bytes[6]);
    
    int currentIndex = 7;
    // Now loop through the rest of the header until all of the parameters have been
    // consumed/created.
    [self.dictionaryMOTHeaderParameters removeAllObjects];
    while (currentIndex < headerBytes.length)
    {
        JZMotHeaderParameter *parameter = [[JZMotHeaderParameter headerParameter] ProcessMOTHeaderParameter:currentIndex data:headerBytes outIndex:&currentIndex];
        if (parameter == nil) {
            NSLog(@"parameter=nil");
            return;
        }
        
//        Byte *ParameterByteDataPtr = (Byte *)parameter.ParameterByteData.bytes;
        NSString *key = [NSString stringWithFormat:@"%d", parameter.ParameterId];
        if ([self.dictionaryMOTHeaderParameters valueForKey:key] == nil) {
            [self.dictionaryMOTHeaderParameters addEntriesFromDictionary:[NSDictionary dictionaryWithObject:parameter forKey:key]];
        }
        
        switch ((MOTParameterType)parameter.ParameterId)
        {
            case MOTParameterTypeContentName:
                self.ContentName = [(JZMotHeaderParameterText *)parameter ToString];
                break;
            case MOTParameterTypeVersionNumber:
                
                self.Version = ((JZMotHeaderParameterVersion *)parameter).version;;
                break;
                //default:
                //Debugger.Break();
                //break;
            case MOTParameterTypeMimeType:
//                var mimeContents = parameter.ToString().Split('/', ';');
//                if (mimeContents.Length > 0)
//                    MIMEType = mimeContents[0];
//                if (mimeContents.Length > 1)
//                    MIMESubType = mimeContents[1];
                break;
            default:
//                NSLog(@"MOTParameterType default case？");
                break;
        }
    }
    self.ContentName = [NSString stringWithFormat:@"%d %s",self.Version, self.ContentName.UTF8String];
    
    // Mark the header as complete
    self.isHeaderComplete = true;
}

- (NSString *)getStatusText
{
//    NSString *string = [NSString string];
//    
//    [string stringByAppendingFormat:@"Actual Segments Received: %ld", self.segments.count];
////    var segmentList = segments.Keys.ToList();
//    NSArray *keys = [self.segments allKeys];
////    segmentList.Sort();
////    [keys sored]
//    
//    
//    foreach (var segmentId in segmentList)
//    segmentsReceived += segmentId.ToString(CultureInfo.InvariantCulture) + ", ";
//    
//    segmentsReceived = segmentsReceived.TrimEnd().TrimEnd(',');
//    
//    return "Status: " + (IsHeaderComplete ? "Complete" : "Incomplete") + "\r\nSegment Count: " + segments.Count + "\r\nActual Segments Received: " + segmentsReceived;
    
    return @"my header info";
}
@end
