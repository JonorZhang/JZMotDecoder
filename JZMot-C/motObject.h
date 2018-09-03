//
//  motObject.h
//  JZMot-C
//
//  Created by Jonor on 16/7/13.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#ifndef motObject_h
#define motObject_h

#include "typedef.h"
#include "motSegment.h"

typedef struct {
	bool isHeaderComplete;
	uint BodySize;
} JZMotObjectHeader;

typedef struct {
	bool isBodyComplete;
    uint BodyDataLen;
} JZMotObjectBody;

typedef struct {
    bool isObjectComplete;
	ushort transportId;
	JZMotObjectHeader *motObjectHeader;
	JZMotObjectBody *motObjectBody;
} JZMotObject;



void JZMotObjectHeaderProcessSegments(JZMotObjectHeader *self, JZMotSegment *segment);
void JZMotObjectHeaderAddSegment(JZMotObjectHeader *self, JZMotSegment *segment);
void JZMotObjectProcessSegments(JZMotObject *self);
void JZMotObjectBodyAddSegment(JZMotObjectBody *self, JZMotSegment *segment);
void JZMotObjectHeaderFree(JZMotObjectHeader *self);
JZMotObjectHeader *JZMotObjectHeaderCreat(void);
void JZMotObjectBodyFree(JZMotObjectBody *self);
JZMotObjectBody *JZMotObjectBodyCreat(void);
void JZMotObjectFree(JZMotObject *self);
JZMotObject *JZMotObjectCreat(ushort transportId);
void JZMotObjectAddSegment(JZMotObject *self, JZMotSegment *segment);
void JZMotObjectPrintf(JZMotObject *self);



// 全局标记
extern bool gFlagSaveCurrentMotBuffer;



#endif /* motObject_h */

