//
//  motGroup.c
//  JZMot-C
//
//  Created by Jonor on 16/7/13.
//  Copyright © 2016年 Jonor. All rights reserved.
//
#include "motGroup.h"
#include "motCheck.h"

void JZMotGroupFree(JZMotGroup *self) {
	if (self == NULL) {
		return;
	}
	JZDataFree(self->EndUserAddressField);
	JZDataFree(self->motGroupDataField);
	JZFree (self);
	self = NULL;
}

JZMotGroup *JZMotGroupCreat(char *groupData, uint len)
{
	// First make sure that the array has enough length for the minimum size packet.
    if (len < 5) {
        JZPrintf("The group data did not have enough bytes to be a proper data group.");
        return NULL;
    }

    JZMotGroup *motGroup = (JZMotGroup *)JZAlloc(1, sizeof(JZMotGroup));
    if (motGroup == NULL) {
    	return NULL;
    }
    
    // Parse out the info we can out of the header.
    uint currentIndex = 0;
    motGroup->ExtensionFlag = (groupData[currentIndex] & 0x80) == 0x80;
    motGroup->CRCFlag = (groupData[currentIndex] & 0x40) == 0x40;
    motGroup->SegmentFlag = (groupData[currentIndex] & 0x20) == 0x20;
    motGroup->UserAccessFlag = (groupData[currentIndex] & 0x10) == 0x10;
    motGroup->motGroupType = (MotGroupTypes)(groupData[currentIndex] & 0x0F);
    currentIndex++;
    motGroup->ContinuityIndex = (Byte)((groupData[currentIndex] & 0xF0) >> 4);
    motGroup->RepetitionIndex = (Byte)(groupData[currentIndex] & 0x0F);
    currentIndex++;
    
    if (motGroup->ExtensionFlag)
    {
        if (currentIndex + 1 >= len) {
            JZPrintf("motGroup->ExtensionFlag did not have enough bytes.");
            goto ERROR_HENDLER;
        }
        
        motGroup->ExtensionField = (ushort)((groupData[currentIndex] << 8) | groupData[currentIndex + 1]);
        currentIndex += 2;
    }
    
    if (motGroup->SegmentFlag)
    {
        if (currentIndex + 1 >= len) {
            JZPrintf("motGroup->SegmentFlag did not have enough bytes.");
            goto ERROR_HENDLER;
        }
        motGroup->SegmentLast = (groupData[currentIndex] & 0x80) == 0x80;
        motGroup->MOTSegmentNumber = (Byte)(((groupData[currentIndex] & 0x7F) << 8) | groupData[currentIndex + 1]);
        currentIndex += 2;
    }
    
    if (motGroup->UserAccessFlag)
    {
        if (currentIndex >= len) {
            JZPrintf("motGroup->UserAccessFlag did not have enough bytes.");
            goto ERROR_HENDLER;
        }
        
        motGroup->TransportIdFlag = (groupData[currentIndex] & 0x10) == 0x10;
        motGroup->UserAccessLength = (Byte)(groupData[currentIndex] & 0x0F);
        currentIndex++;
        
        uint addressFieldLength = motGroup->UserAccessLength;
        
        if (motGroup->TransportIdFlag)
        {
            if (currentIndex + 1 >= len) {
                JZPrintf("motGroup->TransportIdFlag did not have enough bytes.");
                goto ERROR_HENDLER;
            }
            
            motGroup->TransportId = (ushort)((groupData[currentIndex] << 8) | groupData[currentIndex + 1]);
            addressFieldLength -= 2;
            currentIndex += 2;
        }
        
        if (addressFieldLength > 0)
        {
            if (currentIndex + addressFieldLength >= len) {
                JZPrintf("addressFieldLength did not have enough bytes.");
                goto ERROR_HENDLER;
            }
            
            motGroup->EndUserAddressField = JZDataCreat(groupData + currentIndex, addressFieldLength);
            if (motGroup->EndUserAddressField == NULL) {
                goto ERROR_HENDLER;
            }
            currentIndex += addressFieldLength;
        }
    }
  
    // Now calculate the length of the data group data field
    uint motGroupDataFieldLength = len - currentIndex;
  
    // If there is a CRC get it now.
    if (motGroup->CRCFlag)
    {
        if (currentIndex + 1 >= len) {
            JZPrintf("motGroup->CRCFlag did not have enough bytes.");
            goto ERROR_HENDLER;
        }
        motGroup->CRC = (ushort)((groupData[len - 2] << 8) | groupData[len - 1]);        
        motGroupDataFieldLength -= 2;
    }

    // Now copy the data into the data buffer.
    if (motGroupDataFieldLength > 0)
    {
        if ((len - currentIndex) < motGroupDataFieldLength) {
            motGroupDataFieldLength = (len - currentIndex);
        }
        motGroup->motGroupDataField = JZDataCreat(groupData + currentIndex, motGroupDataFieldLength);
        if (motGroup->motGroupDataField == NULL) {
            goto ERROR_HENDLER;
        }
        currentIndex += motGroupDataFieldLength;
    }

    if (motGroup->CRCFlag) {
       JZPrintf("CrcCheck[ConIdx:%2d SegNbr:%2d]", motGroup->ContinuityIndex, motGroup->MOTSegmentNumber);
       if (!checkCRC(groupData, len-2, motGroup->CRC)) {
           JZPrintf("      >_<  CRC Mismatch !!!");
           goto ERROR_HENDLER;
       }
    }
    // 成功创建
    return motGroup;

ERROR_HENDLER:
    JZMotGroupFree(motGroup);
    return NULL;
}

