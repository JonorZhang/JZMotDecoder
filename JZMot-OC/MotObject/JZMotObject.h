//
//  JZMotObject.h
//  mot
//
//  Created by Jonor on 16/7/6.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#import <Foundation/Foundation.h>

@class JZMotObjectHeader;
@class JZMotObjectBody;
@class JZMotSegment;

@interface JZMotObject : NSObject

@property (nonatomic, strong) JZMotObjectHeader *motObjectHeader;
@property (nonatomic, strong) JZMotObjectBody *motObjectBody;
@property (nonatomic, assign) ushort transportId;
//@property (nonatomic, assign) Byte version;


+ (instancetype)objectWithTransportId:(ushort)transportId;
- (void)addSegment:(JZMotSegment *)segment;
- (bool)isObjectComplete;

@end
