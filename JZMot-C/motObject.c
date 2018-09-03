//
//  motObject.c
//  JZMot-C
//
//  Created by Jonor on 16/7/13.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#include "motObject.h"

// 全局标记
bool gFlagSaveCurrentMotBuffer = NO;


//==================== object->header ===================================================

void JZMotObjectHeaderProcessSegments(JZMotObjectHeader *self, JZMotSegment *segment) {
    uint length = segment->segmentBytesData->length;
    Byte *bytes = segment->segmentBytesData->bytes;
    
    // Now see if we have enough bytes in the array to get the header size.
    if (length < 6) return;

    ushort headerSize = (ushort)(((bytes[3] & 0x0F) << 9) | (bytes[4] << 1) | (bytes[5] >> 7));
    
    // Now if there is enough bytes total to process the header then do it.
    if (headerSize > length) return;
    
    // There is enough bytes to process the header so do it.
    self->BodySize = (uint)((bytes[0] << 20) | (bytes[1] << 12) | (bytes[2] << 4) | (bytes[3] >> 4));
    self->isHeaderComplete = true;
}

void JZMotObjectHeaderAddSegment(JZMotObjectHeader *self, JZMotSegment *segment) {
    gFlagSaveCurrentMotBuffer = YES;        
    if (!self->isHeaderComplete) {
        JZMotObjectHeaderProcessSegments(self, segment);
    }
}

void JZMotObjectHeaderFree(JZMotObjectHeader *self) {
    if (self == NULL) {
      return;
  }
  JZFree(self);
  self = NULL;
}

JZMotObjectHeader *JZMotObjectHeaderCreat(void) {
	JZMotObjectHeader *motObjectHeader = (JZMotObjectHeader *)JZAlloc(1, sizeof(JZMotObjectHeader));
	if (motObjectHeader == NULL) {
		return NULL;
	}
    motObjectHeader->BodySize = 0;
    motObjectHeader->isHeaderComplete = false;
    
    return motObjectHeader;
}

//==================== object->body ===================================================

void JZMotObjectBodyAddSegment(JZMotObjectBody *self, JZMotSegment *segment) {
    gFlagSaveCurrentMotBuffer = YES;   
    self->BodyDataLen += segment->segmentBytesSize;
    return; 
}

void JZMotObjectBodyFree(JZMotObjectBody *self) {
	if (self == NULL) {
		return;
	}
	JZFree(self);
	self = NULL;
}

JZMotObjectBody *JZMotObjectBodyCreat(void) {
	JZMotObjectBody *motObjectBody = (JZMotObjectBody *)JZAlloc(1, sizeof(JZMotObjectBody));
	if (motObjectBody == NULL) {
		return NULL;
	}
    motObjectBody->BodyDataLen = 0;
    motObjectBody->isBodyComplete = false;
    
    return motObjectBody;
}

//==================== object ===================================================

void JZMotObjectProcessSegments(JZMotObject *self) {
    if (!self->motObjectHeader->isHeaderComplete) return;
    
    if (self->motObjectBody->BodyDataLen >= self->motObjectHeader->BodySize) {
        self->motObjectBody->isBodyComplete = true;
        self->isObjectComplete = true;
    }
}

void JZMotObjectFree(JZMotObject *self) {
	if (self == NULL) {
		return;
	}
	JZMotObjectHeaderFree(self->motObjectHeader);
	JZMotObjectBodyFree(self->motObjectBody);
	JZFree(self);
	self = NULL;
}

JZMotObject *JZMotObjectCreat(ushort transportId) {
	JZMotObject *motObject = (JZMotObject *)JZAlloc(1, sizeof(JZMotObject));
	if(motObject == NULL) {
		goto ERROR_HENDLER;
	}
	motObject->motObjectHeader = JZMotObjectHeaderCreat();
	if (motObject->motObjectHeader == NULL) {
		goto ERROR_HENDLER;
	}
	motObject->motObjectBody = JZMotObjectBodyCreat();
	if (motObject->motObjectBody == NULL) {
		goto ERROR_HENDLER;
	}
    motObject->isObjectComplete = false;
    motObject->transportId = transportId;
    return motObject;

ERROR_HENDLER:
    JZMotObjectFree(motObject);
    return NULL;
}

void JZMotObjectAddSegment(JZMotObject *self, JZMotSegment *segment)
{
    // Determine if it is a header or body.
    switch (segment->motGroup->motGroupType)
    {
    case MotGroupTypesMotHeaderInfo:
        JZMotObjectHeaderAddSegment(self->motObjectHeader, segment);
        break;
    case MotGroupTypesMotDataAndCaParameters:
    case MotGroupTypesMotData:
        JZMotObjectBodyAddSegment(self->motObjectBody, segment);
        break;
    default:
        // JZPrintf("What data group type are we not handling?");
        break;
    }
    //
    JZMotObjectProcessSegments(self);
}

void JZMotObjectPrintf(JZMotObject *self) {
    if (self == NULL) return;
    JZPrintf("\n\ntransportId:%d", self->transportId);
    
    JZPrintf("JZMotObjectHeader:");
    if (self->motObjectHeader == NULL) goto JZMotObjectBody;    
    JZPrintf("	isHeaderComplete:%d", self->motObjectHeader->isHeaderComplete);
    JZPrintf("	BodySize:%d", self->motObjectHeader->BodySize);

    JZMotObjectBody:
    JZPrintf("JZMotObjectBody:");
    if (self->motObjectBody == NULL) return;   
    JZPrintf("	isBodyComplete:%d", self->motObjectBody->isBodyComplete);
    JZPrintf("	BodyDataLen:%d", self->motObjectBody->BodyDataLen);
}



