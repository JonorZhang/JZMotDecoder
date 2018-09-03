//
//  motGroup.h
//  JZMot-C
//
//  Created by Jonor on 16/7/13.
//  Copyright © 2016年 Jonor. All rights reserved.
//

 #ifndef motGroup_h
#define motGroup_h

#include "typedef.h"

typedef enum {
    MotGroupTypesUnknown = 0xFF,
    MotGroupTypesMotHeaderInfo = 3,
    MotGroupTypesMotData = 4,
    MotGroupTypesMotDataAndCaParameters = 5,
    MotGroupTypesMotDirectory = 6
} MotGroupTypes;

typedef struct {
	bool ExtensionFlag;
	bool CRCFlag;
	bool SegmentFlag;
	bool UserAccessFlag;
	MotGroupTypes motGroupType;
	Byte ContinuityIndex;
	Byte RepetitionIndex;
	ushort ExtensionField;
	bool SegmentLast;
	ushort MOTSegmentNumber;
	bool TransportIdFlag;
	Byte UserAccessLength;
	ushort TransportId;
	JZData *EndUserAddressField;
	ushort CRC;
	JZData *motGroupDataField;
} JZMotGroup;


void JZMotGroupFree(JZMotGroup *self);
JZMotGroup *JZMotGroupCreat(char *groupData, uint len);


#endif /* motGroup_h */

