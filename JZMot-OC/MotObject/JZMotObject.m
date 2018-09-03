//
//  JZMotObject.m
//  mot
//
//  Created by Jonor on 16/7/6.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#import "JZMotObject.h"
#import "JZMotSegment.h"
#import "JZMotObjectHeader.h"
#import "JZMotObjectBody.h"

@implementation JZMotObject

+ (instancetype)objectWithTransportId:(ushort)transportId {
    JZMotObject *object = [[JZMotObject alloc] init];
    object.motObjectHeader = [JZMotObjectHeader header];
    object.motObjectBody = [JZMotObjectBody body];
    object.transportId = transportId;
    
    return object;
}

- (void)addSegment:(JZMotSegment *)segment
{
    // Determine if it is a header or body.
    switch (segment.dataGroup.DataGroupType)
    {
        case DataGroupTypesMotHeaderInfo:
            [self.motObjectHeader addSegment:segment];
            
            if (self.motObjectHeader.isHeaderComplete)
            {
                [self.motObjectBody processSegments:self.motObjectHeader.BodySize];
                //Version = MOTObjectHeader.Version;
            }
            break;
        case DataGroupTypesMotDataAndCaParameters:
        case DataGroupTypesMotData:
            [self.motObjectBody addSegment:segment];
            break;
        default:
            NSLog(@"What data group type are we not handling?");
            break;
    }
}

- (bool)isObjectComplete
{
    return self.motObjectHeader.isHeaderComplete && self.motObjectBody.isBodyComplete;
}

- (NSString *)getStatusText
{
    NSString *string = [NSString string];
    [string stringByAppendingFormat:@"Transport ID: 0x%4x", self.transportId];
    [string stringByAppendingFormat:@"Status: %s", (self.isObjectComplete ? "Complete" : "Incomplete")];
    [string stringByAppendingFormat:@"Header Info:%s", [[self.motObjectHeader getStatusText] UTF8String]];
    [string stringByAppendingFormat:@"Body Info:%s", [[self.motObjectBody getStatusText] UTF8String]];

    return string;
}


@end
