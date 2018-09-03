//
//  JZMotData.m
//  mot
//
//  Created by Jonor on 16/7/5.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#import "JZMotData.h"
#import "JZMotDataGroup.h"

@interface JZMotData ()

@end


@implementation JZMotData


+ (instancetype)data {
    JZMotData *motData = [[JZMotData alloc] init];
    motData.hexArray = [NSMutableArray array];
    
    return motData;
}

+ (instancetype)dataWithPath:(NSString *)path componentsSeparatedByCharactersInSet:(NSCharacterSet *)separator{
    JZMotData *motData = [JZMotData data];
    
    // 连续字符串
    NSMutableData *tmpSlsData = [NSMutableData dataWithContentsOfFile:path];
    // 分组hex序列
    motData.hexArray = [motData stringToHexArray:(char *)tmpSlsData.bytes componentsSeparatedByCharactersInSet:separator];
    NSLog(@"motData.hexArray.count = %ld total", (unsigned long)motData.hexArray.count);

    return motData;
}

+ (instancetype)dataWithCharData:(NSData *)charData componentsSeparatedByCharactersInSet:(NSCharacterSet *)separator{
    JZMotData *motData = [JZMotData data];
    
    // 分组hex序列
    motData.hexArray = [motData stringToHexArray:(char *)charData.bytes componentsSeparatedByCharactersInSet:separator];
    NSLog(@"motData.hexArray.count = %ld total", (unsigned long)motData.hexArray.count);
    
    return motData;
}

+ (instancetype)dataWithHexData:(NSData *)hexData{
    JZMotData *motData = [JZMotData data];
    
    Byte *hexDataPtr = (Byte *)hexData.bytes;
    NSInteger dataLen = 0;
    NSData *hexSegmentData = nil;
    for (NSUInteger index=0; index<hexData.length; ) {
        dataLen = (hexDataPtr[index] << 8) | hexDataPtr[index+1];
        index += 2;
        hexSegmentData = [NSData dataWithBytes:(hexDataPtr + index) length:dataLen];
        index += dataLen;
        [motData.hexArray addObject:hexSegmentData];
//        NSLog(@"hexSegmentData: %@", hexSegmentData);
    }
    
    NSLog(@"motData.hexArray.count = %ld total", (unsigned long)motData.hexArray.count);
    return motData;
}


//将字符串转为十六进制 分组
- (NSMutableArray *)stringToHexArray:(char *)str componentsSeparatedByCharactersInSet:(NSCharacterSet *)separator
{
    NSString *dataString = [NSString stringWithUTF8String:str];
    NSMutableArray *strSegmentStringArray = [NSMutableArray array];
    NSMutableArray *hexSegmentDataArray = [NSMutableArray array];
    
    if (separator != nil) {
        strSegmentStringArray = (NSMutableArray *)[dataString componentsSeparatedByCharactersInSet:separator];
    } else {
        [strSegmentStringArray addObject:dataString];
    }
    
    for (NSString *oneSegmentOfString in strSegmentStringArray) {
        NSMutableData *hexSegmentData = [self stringToHex:(char *)[oneSegmentOfString UTF8String]];
        if (hexSegmentData.length > 0) {
            [hexSegmentDataArray addObject:hexSegmentData];
        }
    }
    
    return hexSegmentDataArray;
}

//将字符串转为十六进制 连续
- (NSMutableData *)stringToHex:(char *)str
{
    int hiHexData, lowHexData;
    int len = (int)strlen(str);
    char hex;
    NSMutableData *hexData;
    
    hexData = [NSMutableData data];
    for(int i=0; i < len; ) {
        // 找到hex数值的开始位置
        while (1) {
            if(!isHexNumber(str[i])) {
                if (i >= len) {
                    break;
                } else {
                    i++;
                }
            } else {
                break;
            }
        }
        
        // 高位前
        hiHexData = convertHexChar(str[i++]);
        // 低位后
        lowHexData = convertHexChar(str[i++]);
        
        if ((hiHexData < 0) || (lowHexData < 0)) {
            break;
        } else {
            hex = (hiHexData * 16 + lowHexData) & 0xFF;
            [hexData appendBytes:&hex length:1];
        }
    }
    return hexData;
}
//
////将字符串转为十六进制 连续
//- (NSMutableData *)charStringToHexString:(NSString *)charString
//{
//    int hiHexData, lowHexData;
//    NSUInteger len = [charString length];
//    Byte *str = (Byte *)charString.UTF8String;
//    NSMutableData *hexData = [NSMutableData data];
//    
//    for(int i=0; i < len; i++) {
//        // 找到hex数值的开始位置
//        Byte byte = str[i];
//        if (/*byte == ' ' || */byte == '\n'){
//            [hexData appendBytes:&byte length:1];
//        } else if(isHexNumber(byte)) {
//            // 高位前
//            hiHexData = convertHexChar(byte);
//            // 低位后
//            byte = str[++i];
//            lowHexData = convertHexChar(byte);
//            
//            if ((hiHexData < 0) || (lowHexData < 0)) {
//                break;
//            } else {
//                byte = (hiHexData * 16 + lowHexData) & 0xFF;
//                [hexData appendBytes:&byte length:1];
//            }
//        }
//    }
//    NSLog(@"%@", hexData);
//    return hexData;
//    
//}

bool isHexNumber(char byte) {
    return ((byte >= '0' && byte <= '9') || (byte >= 'a' && byte <= 'f') || (byte >= 'A' && byte <= 'F'));
}

//功能：若是在0-F之间的字符，则转换为相应的十六进制字符，否则返回-1
char convertHexChar(char ch)
{
    if ((ch >= '0') && (ch <= '9'))
        return ch - '0';
    else if ((ch >= 'A') && (ch <= 'F'))
        return ch - 'A' + 10;
    else if ((ch >= 'a') && (ch <= 'f'))
        return ch - 'a' + 10;
    else return (-1);
}



@end
