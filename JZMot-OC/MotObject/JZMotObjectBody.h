//
//  JZMotObjectBody.h
//  mot
//
//  Created by Jonor on 16/7/6.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#import <Foundation/Foundation.h>

@class JZMotSegment;


@interface JZMotObjectBody : NSObject


@property (nonatomic, strong) NSMutableData *BodyData;
@property (nonatomic, assign) bool isBodyComplete;

+ (instancetype)body;
- (void)processSegments:(uint)bodySize;
- (void)addSegment:(JZMotSegment *)segment;
- (NSString *)getStatusText;

@end
