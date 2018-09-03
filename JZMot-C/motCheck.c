//
//  motCheck.c
//  JZMot-C
//
//  Created by Jonor on 16/7/13.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#include "motCheck.h"

bool checkCRC(Byte *data, uint len, ushort crc)
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
    
    uint index = 0;
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
    if (onesComplement) {
        // ReSharper restore ConditionIsAlwaysTrueOrFalse
        shiftRegister = (ushort)(~shiftRegister);
    }
    
    return (shiftRegister == crc);
}
