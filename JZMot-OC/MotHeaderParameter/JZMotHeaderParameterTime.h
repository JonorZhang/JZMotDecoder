//
//  JZMotHeaderParameterTime.h
//  mot
//
//  Created by Jonor on 16/7/7.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "JZMotHeaderParameter.h"

@interface JZMotHeaderParameterTime  : JZMotHeaderParameter

@property (nonatomic, assign) bool IsNow;
@property (nonatomic, strong) NSDate *time;

+ (instancetype)time;

@end
