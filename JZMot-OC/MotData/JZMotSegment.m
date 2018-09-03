//
//  JZMotSegment.m
//  mot
//
//  Created by Jonor on 16/7/5.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#import "JZMotSegment.h"

@interface JZMotSegment ()

@end


@implementation JZMotSegment

+ (instancetype)segment:(JZMotDataGroup *)dataGroup
{
    JZMotSegment *motSegment = [[JZMotSegment alloc] init];
    
    motSegment.dataGroup = dataGroup;
    NSMutableData *dataGroupDataField = dataGroup.DataGroupDataField;
    Byte *dataGroupDataFieldBytes = (Byte *)dataGroupDataField.bytes;
    
    // First get the segment header values.
    if (dataGroupDataField.length <= 0) {
        NSLog(@"dataGroupDataField.length was not large enough to parse segment.");
        return nil;
    }
    
    int currentIndex = 0;
    motSegment.repetitionCount = dataGroupDataFieldBytes[currentIndex] >> 5;
    motSegment.segmentBytesSize = (ushort)(((dataGroupDataFieldBytes[currentIndex] & 0x1F) << 8) | dataGroupDataFieldBytes[currentIndex + 1]);
    currentIndex += 2;
    
    // Make sure there was enough data to cover the segment size.
    if (motSegment.segmentBytesSize > dataGroupDataField.length - currentIndex) {
        NSLog(@"motSegment.segmentBytesSize was not large enough to parse segment.");
        return nil;
    }
    
    // Now comment the data based on the segment size.
    motSegment.segmentBytesData = [NSMutableData data];
    Byte *bytesPtr = dataGroupDataFieldBytes + currentIndex;
    NSInteger bytesLen = dataGroupDataField.length-currentIndex;
    [motSegment.segmentBytesData appendBytes:bytesPtr length:bytesLen];
    
    return motSegment;
}


@end
