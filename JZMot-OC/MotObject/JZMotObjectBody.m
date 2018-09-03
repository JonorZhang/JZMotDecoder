//
//  JZMotObjectBody.m
//  mot
//
//  Created by Jonor on 16/7/6.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#import "JZMotObjectBody.h"
#import "JZMotSegment.h"

@interface JZMotObjectBody ()

@property (nonatomic, assign) ushort nextSegmentToProcess;
@property (nonatomic, assign) NSInteger nextIndex;
// <ushort, MOTSegment>
@property (nonatomic, strong) NSMutableDictionary *segments;

@end

@implementation JZMotObjectBody

+ (instancetype)body {
    JZMotObjectBody *body = [[JZMotObjectBody alloc] init];
    body.segments = [NSMutableDictionary dictionary];
    body.BodyData = [NSMutableData data];
    
    return body;
}

- (void)addSegment:(JZMotSegment *)segment
{
    // Determine if the segment has been already received.
    // If it has not already been received then add it to the hash.
    NSString *key = [NSString stringWithFormat:@"%d", segment.dataGroup.MOTSegmentNumber];
    if ([self.segments valueForKey:key] != nil) {
        return;
    }
    
    [self.segments addEntriesFromDictionary:[NSDictionary dictionaryWithObject:segment forKey:key]];
    
    if (!self.isBodyComplete && self.BodyData.length > 0)
        [self processSegments];
    
}

- (void)processSegments:(uint)bodySize
{
    if (self.isBodyComplete)
        return;
    
    if (self.BodyData == nil || self.BodyData.length != bodySize)
    {
        self.BodyData = [NSMutableData dataWithLength:bodySize]; //这里要开空间了，否则会数据错，未查明原因
//        self.BodyData = [NSMutableData data];
        self.nextSegmentToProcess = 0;
        self.nextIndex = 0;
        self.isBodyComplete = false;
    }
    
    [self processSegments];
}

- (void)processSegments
{
    JZMotSegment *segment;
    // Loop through the available segments and process them as they get populated
    while ((segment = [self.segments valueForKey:[NSString stringWithFormat:@"%d", self.nextSegmentToProcess]]) != nil)
    {
        // Get the segment
//        JZMotSegment *segment = segments[nextSegmentToProcess];
        
        // Add the bytes to the header bytes array list
        // Now loop through and copy the elements from the segment into the body.
//        [self.BodyData appendBytes:segment.segmentBytesData.bytes length:segment.segmentBytesData.length];
        [self.BodyData replaceBytesInRange:NSMakeRange(self.nextIndex, segment.segmentBytesData.length) withBytes:segment.segmentBytesData.bytes];
        
        // Go to the next segment
        self.nextSegmentToProcess++;
        self.nextIndex += segment.segmentBytesData.length;
    }
    
    if (self.nextIndex >= self.BodyData.length)
        self.isBodyComplete = true;
}

- (NSString *)getStatusText
{
    
//    var segmentsReceived = segments.Count > 0 ? "" : "None";
//    var segmentList = segments.Keys.ToList();
//    segmentList.Sort();
//    foreach(var segmentId in segmentList)
//    segmentsReceived += segmentId.ToString(CultureInfo.InvariantCulture) + ", ";
//    
//    segmentsReceived = segmentsReceived.TrimEnd().TrimEnd(',');
//    
//    return "Status: " + (BodyComplete ? "Complete" : "Incomplete") + "\r\nSegment Count: " + segments.Count + "\r\nActual Segments Received: " + segmentsReceived;
    return @"my body info";
}

@end
