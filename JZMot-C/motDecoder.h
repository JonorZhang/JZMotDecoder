//
//  motDecoder.h
//  JZMot-C
//
//  Created by Jonor on 16/7/13.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#ifndef motDecoder_h
#define motDecoder_h

/** 
 * in -> [                       motDcoder                               ] -> out
 * in -> [ motData -> checkData -> motGroup -> motSegment -> motObject ] -> out
 */
#include "typedef.h"
#include "motGroup.h"
#include "motSegment.h"
#include "motObject.h"


//====================== Class =======================================================
typedef struct _privateMember PrivateMember;
typedef struct MotDecoder JZMotDecoder;
struct MotDecoder{
    JZMotData *motData;
	JZDictionary *motObjectsTransportIdLookup;
    ushort currentTansportID;

    void (*processData)(JZMotDecoder *self, char *buf, uint len);
	JZData *(*getMotData)(JZMotDecoder *self);
};


//======================= Variable ======================================================
extern JZMotDecoder *motDecoder;


//======================= Function ======================================================
JZMotDecoder *newMotDecoder(void);
void freeMotDecoder(JZMotDecoder *self);




#endif /* motDecoder_h */
