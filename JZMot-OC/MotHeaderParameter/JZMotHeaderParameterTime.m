//
//  JZMotHeaderParameterTime.m
//  mot
//
//  Created by Jonor on 16/7/7.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#import "JZMotHeaderParameterTime.h"

@implementation JZMotHeaderParameterTime

+ (instancetype)time {
    JZMotHeaderParameterTime *parameterTime = [[JZMotHeaderParameterTime alloc] init];
    return parameterTime;
}

- (void)ProcessData
{
    Byte *ParameterByteDataPtr = (Byte *)self.ParameterByteData.bytes;
    //base.ProcessData();
    
    // Make sure there is enough data to process.
    if (self.ParameterByteData.length < 1)
        NSLog(@"Not enough data associated with the parameter to process the time.");
    
    // See if the time is now.
    if ((ParameterByteDataPtr[0] & 0x80) == 0x00)
        self.IsNow = true;
    
    if (self.IsNow)
        return;
    
    // Figure out if we have enough bytes.
    if (self.ParameterByteData.length < 4)
        NSLog(@"Not enough data associated with the parameter to process the time.");
    
// 不知是什么鬼
//    // First decode the julian date.
//    ushort modJulianDate = ((ParameterByteDataPtr[0] & 0x7F) << 10) | (ParameterByteDataPtr[1] << 2) | (ParameterByteDataPtr[2] >> 6);
//    
//    // Determine if it is an extended UTC format
//    ushort extendedUTC = (ParameterByteDataPtr[2] & 0x10) == 0x10;
//    
//    ushort utcHour = ((ParameterByteDataPtr[2] & 0x0F) << 1) | (ParameterByteDataPtr[3] >> 7);
//    ushort utcMinute = ((ParameterByteDataPtr[3] >> 1) & 0x3F);
//    ushort utcSecond = 0;
//    ushort utcMillisecond = 0;
//    if (extendedUTC)
//    {
//        // Figure out if we have enough bytes.
//        if (self.ParameterByteData.length < 6)
//            NSLog(@"Not enough data associated with the parameter to process the time.");
//        
//        // Extended UTC also contains the seconds and milliseconds.
//        utcSecond = ((ParameterByteDataPtr[3] & 0x01) << 5) | (ParameterByteDataPtr[4] >> 3);
//        utcMillisecond = ((ParameterByteDataPtr[4] & 0x07) << 7) | (ParameterByteDataPtr[5] >> 1);
//    }
//    // Modified Julian is from the following date.
//    NSDate *dateTime = [NSDate dateWithTimeIntervalSince1970:0];
//    
//    dateTime = dateTime.AddDays(modJulianDate - 40587);
//    DateTime = dateTime.Add(new TimeSpan(0, utcHour, utcMinute, utcSecond, utcMillisecond));
}

@end
