//
//  JZMotDataGroup.m
//  AppCast
//
//  Created by Jonor on 16/6/16.
//  Copyright © 2016年 soundmax. All rights reserved.
//

#import "JZMotDataGroup.h"

@interface JZMotDataGroup ()
@end

@implementation JZMotDataGroup

- (BOOL)checkCRC:(Byte *)data length:(NSInteger)len crc:(ushort)crc
{
    /* Configuration items for the CRC Check */
    // CRC Poly for this implementation is G(x)=x^16 + x^12 + x^5 + 1
    const uint crcPoly = ((1 << 16) | (1 << 12) | (1 << 5) | (1 << 0));
    
    // Do a ones complement on the item before it is returned.
    const bool onesComplement = true;
    
    // Initial shift register contents.
    ushort shiftRegister = 0xFFFF;
    /* END OF CONFIGURATION */
    
    // Now calculate the CRC.
    // Loop through all the data and shift it through the CRC shift register.
   
    NSInteger index = 0;
    int item = 0;
    Byte element = 0;
    Byte bit = 0;
    ushort invert = 0;
    for (index = 0; index < len; index++) {
        element = data[index];
        for (item = 7; item >= 0; item--)
        {
            bit = (element >> item) & 0x01;
            
            // Figure out the direction of the XOR based on the bit
            invert = (ushort)(bit ^ ((shiftRegister & 0x8000) == 0x8000 ? 0x0001 : 0x0000));
            
            // Now shift the shift register by 1
            shiftRegister = (ushort)(shiftRegister << 1);
            
            // Now if invert is a 1 then bitwise or the CRCPoly to the main shift register.
            if (invert == 0x0001)
                shiftRegister = (ushort)(shiftRegister ^ crcPoly);
        }
    }
        
    
    // Now the shift register should contain the actual CRC value.
    // Do a one's compliment on the CRC register if necessary.
    // ReSharper disable ConditionIsAlwaysTrueOrFalse
    if (onesComplement)
        // ReSharper restore ConditionIsAlwaysTrueOrFalse
        shiftRegister = (ushort)(~shiftRegister);
    
    return shiftRegister == crc;
}

+ (instancetype)dataGroup:(Byte *)groupData length:(NSInteger)len checkCRC:(BOOL)checkCRC
{
    JZMotDataGroup *dataGroup = [[JZMotDataGroup alloc] init];
    
    
    dataGroup.EndUserAddressField = [NSMutableData data];
    dataGroup.DataGroupDataField  = [NSMutableData data];
    
    // First make sure that the array has enough length for the minimum size packet.
    if (len < 5) {
        NSLog(@"The group data did not have enough bytes to be a proper data group.");
        return nil;
    }
    
    // Parse out the info we can out of the header.
    NSInteger currentIndex = 0;
    dataGroup.ExtensionFlag = (groupData[currentIndex] & 0x80) == 0x80;
    dataGroup.CRCFlag = (groupData[currentIndex] & 0x40) == 0x40;
    dataGroup.SegmentFlag = (groupData[currentIndex] & 0x20) == 0x20;
    dataGroup.UserAccessFlag = (groupData[currentIndex] & 0x10) == 0x10;
    dataGroup.DataGroupType = (DataGroupTypes)(groupData[currentIndex] & 0x0F);
    currentIndex++;
    dataGroup.ContinuityIndex = (Byte)((groupData[currentIndex] & 0xF0) >> 4);
    dataGroup.RepetitionIndex = (Byte)(groupData[currentIndex] & 0x0F);
    currentIndex++;
    
    if (dataGroup.ExtensionFlag)
    {
        if (currentIndex + 1 >= len) {
            NSLog(@"dataGroup.ExtensionFlag did not have enough bytes.");
            return nil;
        }
        
        dataGroup.ExtensionField = (ushort)((groupData[currentIndex] << 8) | groupData[currentIndex + 1]);
        currentIndex += 2;
    }
    
    if (dataGroup.SegmentFlag)
    {
        if (currentIndex + 1 >= len) {
            NSLog(@"dataGroup.SegmentFlag did not have enough bytes.");
            return nil;
        }
        dataGroup.SegmentLast = (groupData[currentIndex] & 0x80) == 0x80;
        dataGroup.MOTSegmentNumber = (Byte)(((groupData[currentIndex] & 0x7F) << 8) | groupData[currentIndex + 1]);
        currentIndex += 2;
    }
    
    if (dataGroup.UserAccessFlag)
    {
        if (currentIndex >= len) {
            NSLog(@"dataGroup.UserAccessFlag did not have enough bytes.");
            return nil;
        }
        
        dataGroup.TransportIdFlag = (groupData[currentIndex] & 0x10) == 0x10;
        dataGroup.UserAccessLength = (Byte)(groupData[currentIndex] & 0x0F);
        currentIndex++;
        
        NSInteger addressFieldLength = dataGroup.UserAccessLength;
        
        if (dataGroup.TransportIdFlag)
        {
            if (currentIndex + 1 >= len) {
                NSLog(@"dataGroup.TransportIdFlag did not have enough bytes.");
                return nil;
            }
            
            dataGroup.TransportId = (ushort)((groupData[currentIndex] << 8) | groupData[currentIndex + 1]);
            addressFieldLength -= 2;
            currentIndex += 2;
        }
        
        if (addressFieldLength > 0)
        {
            if (currentIndex + addressFieldLength >= len) {
                NSLog(@"addressFieldLength did not have enough bytes.");
                return nil;
            }
            
            dataGroup.EndUserAddressField = [[NSMutableData alloc] init];
            
            for (NSInteger i = 0; i < addressFieldLength; i++, currentIndex++)
                [dataGroup.EndUserAddressField appendBytes:(groupData + currentIndex) length:1];
        }
    }
    
    // Now calculate the length of the data group data field
    NSInteger dataGroupDataFieldLength = len - currentIndex;
    
    // If there is a CRC get it now.
    if (dataGroup.CRCFlag)
    {
        if (currentIndex + 1 >= len) {
            NSLog(@"dataGroup.CRCFlag did not have enough bytes.");
            return nil;
        }
        
        dataGroup.CRC = (ushort)((groupData[len - 2] << 8) | groupData[len - 1]);
        
        dataGroupDataFieldLength -= 2;
    }
    
    // Now copy the data into the data buffer.
    if (dataGroupDataFieldLength > 0)
    {
        
        dataGroup.DataGroupDataField = [[NSMutableData alloc] init];
        
        for (NSInteger i = 0; i < dataGroupDataFieldLength && currentIndex < len; i++, currentIndex++)
            [dataGroup.DataGroupDataField appendBytes:(groupData + currentIndex) length:1];
    }
    
    if (!checkCRC || !dataGroup.CRCFlag) {
        return dataGroup;
    }
    
    NSLog(@"dataGroup[ContinuityIndex:%2d SegmentNumber:%2d] CRC checking...", dataGroup.ContinuityIndex, dataGroup.MOTSegmentNumber);
    if (![dataGroup checkCRC:groupData length:len-2 crc:dataGroup.CRC]) {
        NSLog(@"      >_<  Mismatch !!!");
        return nil;
    }
    
    return dataGroup;
}

@end
