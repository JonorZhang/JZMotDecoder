//
//  JZMotHeaderParameterText.m
//  mot
//
//  Created by Jonor on 16/7/8.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#import "JZMotHeaderParameterText.h"

@interface JZMotHeaderParameterText ()

@property (nonatomic, assign) MOTTextCharacterSet CharacterSet;
@property (nonatomic, strong) NSMutableData *CharArray;
@property (nonatomic, assign) ushort CharacterFlag; // This is only used for the label.

@end


@implementation JZMotHeaderParameterText

+ (instancetype)text {
    JZMotHeaderParameterText *text = [[self alloc] init];
    text.CharArray = [NSMutableData data];
    text.CharacterFlag = 0xFFFF;

    return text;
}

- (void)ProcessData
{
    //base.ProcessData();
    Byte *ParameterByteDataPtr = (Byte *)self.ParameterByteData.bytes;
    
    // Parse the data based on the parameter type.
    if ((MOTParameterType)self.ParameterId == MOTParameterTypeLabel)
    {
        // Make sure there is enough data to process.
        if (self.ParameterByteData.length < 19)
            NSLog(@"Not enough data associated with the parameter to process the label.");
        
        // Get the character set.
        self.CharacterSet = (MOTTextCharacterSet)(ParameterByteDataPtr[0] >> 4);
        
        // Now create the byte array to hold the characters.
        self.CharArray = [NSMutableData dataWithBytes:(ParameterByteDataPtr + 1) length:16];
        
        // Now calculate the characterFlag
        self.CharacterFlag = (ushort)((ParameterByteDataPtr[17] << 8) | (ParameterByteDataPtr[18]));
    }
    else if ((MOTParameterType)self.ParameterId == MOTParameterTypeMimeType)
    {
        self.CharacterSet = MOTTextCharacterSetUTF8;
        
        // Now create the byte array to hold the characters and copy the characters into the array.
        self.CharArray = [NSMutableData dataWithBytes:ParameterByteDataPtr length:self.ParameterByteData.length];
    }
    else
    {
        // Make sure there is enough data to process.
        if (self.ParameterByteData.length < 1)
            NSLog(@"Not enough data associated with the parameter to process the text element.");
        
        // Get the character set.
        self.CharacterSet = (MOTTextCharacterSet)(ParameterByteDataPtr[0] >> 4);
        
        // Now create the byte array to hold the characters and copy the characters into the array.
        self.CharArray = [NSMutableData dataWithBytes:(ParameterByteDataPtr + 1) length:(self.ParameterByteData.length - 1)];
    }
}
    
- (NSString *)ToString
{
    // Based on the encoding type process the byte string.
    NSString *string = [NSString stringWithCharacters:self.CharArray.bytes length:self.CharArray.length];

//    switch (self.CharacterSet)
//    {
//        case MOTTextCharacterSetEBULatinBasedRepertoire:
//            return EBULatinCharacterSet.EBUToString(CharArray);
//        case MOTTextCharacterSetEBULatinBasedCommonCoreCyrilllicGreek:
//            return EBUCommonCoreGreekAndCyrillicCharacterSet.EBUToString(CharArray);
//        case MOTTextCharacterSetEBULatinBasedCoreArabicHebrewCyrillicGreek:
//            return EBULatinGreekHebrewAndArabicCyrillicCharacterSet.EBUToString(CharArray);
//        case MOTTextCharacterSetISOLatinAlphabetNo2:
//            return ISO8859_2CharacterSet.ToString(CharArray);
//        case MOTTextCharacterSetISOLatinAlphabetNo1:
//            return ISO8859_1CharacterSet.ToString(CharArray);
//        case MOTTextCharacterSetUCS2:
//            return Encoding.Unicode.GetString(CharArray);
//        case MOTTextCharacterSetUTF8:
//            return Encoding.UTF8.GetString(CharArray);
//    }
    [string UTF8String];
    return string;
}

@end
