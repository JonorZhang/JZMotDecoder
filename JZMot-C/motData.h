//
//  motData.h
//  JZMot-C
//
//  Created by Jonor on 16/7/13.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#ifndef motData_h
#define motData_h

#include "typedef.h"

typedef struct {
	JZDictionary *tranIdDict; // JZDictionary<id, MotData *>
} JZMotData;


JZData *JZMotDataOutput(JZMotData *self, ushort transportId);
JZDictionary *JZMotDataAdd(JZMotData *self, Byte *buf, uint length, ushort segmentNumber, ushort transportId);
void JZMotDataFree(JZMotData *self);
JZMotData *JZMotDataCreat(void);
void JZMotDataPrintf(JZMotData *self);

#endif /* motData_h */

