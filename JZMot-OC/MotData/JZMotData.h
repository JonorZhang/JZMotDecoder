//
//  JZMotData.h
//  mot
//
//  Created by Jonor on 16/7/5.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface JZMotData : NSObject

@property (nonatomic, strong) NSMutableArray *hexArray;

+ (instancetype)data;
+ (instancetype)dataWithPath:(NSString *)path componentsSeparatedByCharactersInSet:(NSCharacterSet *)separator;
+ (instancetype)dataWithCharData:(NSData *)charData componentsSeparatedByCharactersInSet:(NSCharacterSet *)separator;
+ (instancetype)dataWithHexData:(NSData *)hexData;

//- (NSMutableData *)charStringToHexString:(NSString *)charString;
@end
