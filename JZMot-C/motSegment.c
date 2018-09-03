//
//  motSegment.c
//  JZMot-C
//
//  Created by Jonor on 16/7/13.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#include "motSegment.h"
#include "motGroup.h"

void JZMotSegmentFree(JZMotSegment *self) {
	if (self == NULL) {
		return;
	}
	JZDataFree(self->segmentBytesData);
	// JZMotGroupFree(self->motGroup); // 这个不是自己开的内存,不要自己释放
	JZFree(self);
	self = NULL;
}

JZMotSegment *JZMotSegmentCreat(JZMotGroup *motGroup)
{
	JZMotSegment *motSegment = (JZMotSegment *)JZAlloc(1, sizeof(JZMotSegment));
	if (motSegment == NULL) {
		goto ERROR_HENDLER;
	}
    motSegment->motGroup = motGroup;
    JZData *motGroupDataField = motGroup->motGroupDataField;

	// First get the segment header values.
    if (motGroupDataField->length <= 0) {
        JZPrintf("motGroupDataField->length was not large enough to parse segment.");
        goto ERROR_HENDLER;
    }
    
    int currentIndex = 0;
    motSegment->repetitionCount = motGroupDataField->bytes[currentIndex] >> 5;
    motSegment->segmentBytesSize = (ushort)(((motGroupDataField->bytes[currentIndex] & 0x1F) << 8) | motGroupDataField->bytes[currentIndex + 1]);
    currentIndex += 2;
    
    // Make sure there was enough data to cover the segment size.
    if (motSegment->segmentBytesSize > motGroupDataField->length - currentIndex) {
        JZPrintf("motSegment->segmentBytesSize was not large enough to parse segment.");
        goto ERROR_HENDLER;
    }
    
    // Now comment the data based on the segment size.
    Byte *bytesPtr = motGroupDataField->bytes + currentIndex;
    uint bytesLen = motGroupDataField->length - currentIndex;
    motSegment->segmentBytesData = JZDataCreat(bytesPtr, bytesLen);
    if (motSegment->segmentBytesData == NULL) {
        JZPrintf("motSegment->segmentBytesData creat failed.");
        goto ERROR_HENDLER;
    }
    
    return motSegment;

ERROR_HENDLER:
    JZMotSegmentFree(motSegment);
    return NULL;
}

