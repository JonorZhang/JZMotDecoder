//
//  JZMotDirectory.m
//  mot
//
//  Created by Jonor on 16/7/5.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#import "JZMotDirectory.h"

@interface JZMotDirectory ()

// <ushort, MOTSegment>
@property (nonatomic, strong) NSMutableDictionary *motSegments;
// <byte, MOTHeaderParameter>
@property (nonatomic, strong) NSMutableDictionary *motHeaderParametersDirectory;
// <ushort, MOTObjectHeader>
@property (nonatomic, strong) NSMutableDictionary *motObjectHeaders;

@end

@implementation JZMotDirectory

+ (instancetype)directory {
    JZMotDirectory *directory = [[JZMotDirectory alloc] init];
    directory.motSegments = [NSMutableDictionary dictionary];
    directory.motHeaderParametersDirectory = [NSMutableDictionary dictionary];
    directory.motHeaderParametersDirectory = [NSMutableDictionary dictionary];
    directory.contentName = [NSString string];
    
    return directory;
}

@end
