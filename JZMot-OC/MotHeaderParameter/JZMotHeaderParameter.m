//
//  JZMotHeaderParameter.m
//  mot
//
//  Created by Jonor on 16/7/7.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#import "JZMotHeaderParameter.h"
#import "JZMotHeaderParameterTime.h"
#import "JZMotHeaderParameterVersion.h"
#import "JZMotHeaderParameterText.h"

@interface JZMotHeaderParameter ()

@end

@implementation JZMotHeaderParameter

+ (instancetype)headerParameter {
    JZMotHeaderParameter *headerParameter = [[self alloc] init];
    headerParameter.ParameterId = MOTParameterTypeApplicationSpecific;
    headerParameter.ParameterByteData = [NSMutableData data];
    return headerParameter;
}

- (instancetype)ProcessMOTHeaderParameter:(int)startIndex data:(NSData *)byteData outIndex:(int *)nextIndex
{
    if (startIndex >= byteData.length)
        NSLog(@"Error processing the MOT Header Parameter.  Not enough bytes in data stream.");
    Byte *byteDataPtr = (Byte *)byteData.bytes;
    
    // Calculate the PLI and parameter ID out of the first byte.
    Byte parameterId = byteDataPtr[startIndex] & 0x3F;
    JZMotHeaderParameter *motHeaderParameter = nil;
    
    switch ((MOTParameterType)parameterId)
    {
        case MOTParameterTypeCreationTime:
        case MOTParameterTypeStartValidity:
        case MOTParameterTypeExpireTime:
        case MOTParameterTypeTriggerTime:
            motHeaderParameter = [JZMotHeaderParameterTime time];
            break;
        case MOTParameterTypeVersionNumber:
            motHeaderParameter = [JZMotHeaderParameterVersion version];
            break;
            
        case MOTParameterTypeRetransmissionDistance:
//什么鬼/////            motHeaderParameter = new MOTHeaderParameterRetransmissionDistance();
            motHeaderParameter = [JZMotHeaderParameter headerParameter];
           break;
        case MOTParameterTypeGroupReference:
//什么鬼/////            motHeaderParameter = new MOTHeaderParameterGroupReference();
            motHeaderParameter = [JZMotHeaderParameter headerParameter];
            break;
        case MOTParameterTypeLabel:
        case MOTParameterTypeContentName:
        case MOTParameterTypeContentDescription:
        case MOTParameterTypeMimeType:
            motHeaderParameter = [JZMotHeaderParameterText text];
            break;
        case MOTParameterTypePriority:
//什么鬼/////            motHeaderParameter = new MOTHeaderParameterPriority();
            motHeaderParameter = [JZMotHeaderParameter headerParameter];
            break;
            // NEW SPEC PARAMETER TYPES
        case MOTParameterTypePermitOutDatedVersions:
        case MOTParameterTypeExpiration:
        case MOTParameterTypeUniqueBodyVersion:
        case MOTParameterTypeCompressionType:
        case MOTParameterTypeAdditionalHeader:
        case MOTParameterTypeProfileSubset:
        case MOTParameterTypeCAInfo:
        case MOTParameterTypeCAReplacementObject:
            motHeaderParameter = [JZMotHeaderParameter headerParameter];
            break;
        default:
            motHeaderParameter = [JZMotHeaderParameter headerParameter];
            break;
    }
    
    motHeaderParameter.PLI = (Byte)(byteDataPtr[startIndex] >> 6);
    motHeaderParameter.ParameterId = parameterId;
    
    // Based on the PLI type figure out how many bytes are needed.
    int numberOfBytes = 1;
    switch (motHeaderParameter.PLI)
    {
        case 0:
            numberOfBytes = 1;
            break;
        case 1:
            numberOfBytes = 2;
            break;
        case 2:
            numberOfBytes = 5;
            break;
        case 3:
            numberOfBytes = 2; // This is the minimum amount until the length is calculated.
            break;
    }
    
    if (startIndex + numberOfBytes > byteData.length)
        NSLog(@"Error processing the MOT Header Parameter.  Not enough bytes in data stream.");
    
    // If the PLI is three add the amount of bytes needed.
    if (motHeaderParameter.PLI == 3)
    {
        int dataLengthInBytes;
        int index = startIndex + 2;
        // Check to see if the bit is high for the 15 bit length.
        if ((byteDataPtr[startIndex + 1] & 0x80) == 0x80)
        {
            numberOfBytes++; // Increment by one because the size is 15 bits not 7.
            index++;
            dataLengthInBytes = ((byteDataPtr[startIndex + 1] & 0x7F) << 8) | byteDataPtr[startIndex + 2];
        }
        else
            dataLengthInBytes = byteDataPtr[startIndex + 1] & 0x7F;
        
        // Increment the bytes by the data length.
        numberOfBytes += dataLengthInBytes;
        
        // Now copy the data out of the device into the array used to contain the data.
        if (startIndex + numberOfBytes > byteData.length)
            NSLog(@"Error processing the MOT Header Parameter.  Not enough bytes in data stream.");
        
        motHeaderParameter.ParameterByteData = [NSMutableData dataWithBytes:(byteDataPtr + index) length:motHeaderParameter.ParameterByteData.length];
    }
    else if (motHeaderParameter.PLI == 1)
    {
        
        motHeaderParameter.ParameterByteData = [NSMutableData dataWithBytes:(byteDataPtr +startIndex + 1) length:1];
    }
    else if (motHeaderParameter.PLI == 2)
    {
        motHeaderParameter.ParameterByteData = [NSMutableData dataWithBytes:(byteDataPtr + 1) length:4];
    }
    
    [self ProcessData];
    
    *nextIndex = startIndex + numberOfBytes;
    return motHeaderParameter;
}

- (void)ProcessData
{
    
}

@end
