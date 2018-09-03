//
//  JZMotDirectory.h
//  mot
//
//  Created by Jonor on 16/7/5.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface JZMotDirectory : NSObject

@property (nonatomic, assign) ushort transportId;
@property (nonatomic, assign) uint directorySize;
@property (nonatomic, assign) ushort numberOfObjects;
@property (nonatomic, assign) uint carouselPeriod;
@property (nonatomic, assign) ushort segmentSize;
@property (nonatomic, assign) bool directoryComplete;
@property (nonatomic, strong) NSString *contentName;
@property (nonatomic, assign) Byte version;

+ (instancetype)directory;

@end
