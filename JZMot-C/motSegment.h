//
//  motSegment.h
//  JZMot-C
//
//  Created by Jonor on 16/7/13.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#ifndef motSegment_h
#define motSegment_h

#include "typedef.h"
#include "motGroup.h"

typedef struct {
	Byte repetitionCount;
	ushort segmentBytesSize;
	JZData *segmentBytesData;
	JZMotGroup *motGroup;
} JZMotSegment;

void JZMotSegmentFree(JZMotSegment *self);
JZMotSegment *JZMotSegmentCreat(JZMotGroup *data);



#endif /* motSegment_h */

