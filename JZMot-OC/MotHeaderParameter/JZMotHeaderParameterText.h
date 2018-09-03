//
//  JZMotHeaderParameterText.h
//  mot
//
//  Created by Jonor on 16/7/8.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "JZMotHeaderParameter.h"

typedef enum
{
    MOTTextCharacterSetEBULatinBasedRepertoire = 0x0,
    MOTTextCharacterSetEBULatinBasedCommonCoreCyrilllicGreek = 0x1,
    MOTTextCharacterSetEBULatinBasedCoreArabicHebrewCyrillicGreek = 0x2,
    MOTTextCharacterSetISOLatinAlphabetNo2 = 0x3,
    MOTTextCharacterSetISOLatinAlphabetNo1 = 0x4,
    MOTTextCharacterSetUCS2 = 0x6,
    MOTTextCharacterSetUTF8 = 0xF
} MOTTextCharacterSet;


@interface JZMotHeaderParameterText : JZMotHeaderParameter


+ (instancetype)text;
- (void)ProcessData;
- (NSString *)ToString;

@end
