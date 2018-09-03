//
//  JZMotSegment.h
//  mot
//
//  Created by Jonor on 16/7/5.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "JZMotDataGroup.h"


@interface JZMotSegment : NSObject
@property (nonatomic, assign) Byte repetitionCount;
@property (nonatomic, assign) ushort segmentBytesSize;
@property (nonatomic, strong) NSMutableData *segmentBytesData;
@property (nonatomic, strong) JZMotDataGroup *dataGroup;


+ (instancetype)segment:(JZMotDataGroup *)dataGroup;

@end
