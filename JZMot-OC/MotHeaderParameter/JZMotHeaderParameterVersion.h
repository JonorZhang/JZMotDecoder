//
//  JZMotHeaderParameterVersion.h
//  mot
//
//  Created by Jonor on 16/7/6.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "JZMotHeaderParameter.h"

@interface JZMotHeaderParameterVersion  : JZMotHeaderParameter

@property (nonatomic, assign) Byte version;

+ (instancetype)version;

@end
