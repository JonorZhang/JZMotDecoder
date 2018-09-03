//
//  typedef.h
//  JZMot-C
//
//  Created by Jonor on 16/7/13.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#ifndef typedef_h
#define typedef_h


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include <stdbool.h>
typedef unsigned char bool;

// ================== 基本数据类型 ======================
typedef unsigned int uint;  
typedef unsigned short ushort;  
typedef char Byte;   

#define NO  0
#define YES 1
#define false  0
#define true 1
//#define FALSE  0
//#define TRUE 1

typedef struct {
	uint index;
	uint length;
} JZRange;

typedef enum {
	JZDictionaryValueTypeJZData,
	JZDictionaryValueTypeJZMotData,
	JZDictionaryValueTypeJZMotObject,
	JZDictionaryValueTypeJZMotGroup,
	JZDictionaryValueTypeJZMotSegment,
} JZDictionaryValueType;

void memoryTableClear(void);
uint JZMemoryUsedTotal(void);
void *JZAlloc(uint n, uint size);
void *JZRealloc(void *p, uint size);
void JZFree(void *p);


// ================== JZData ======================
typedef struct {
	Byte *bytes;
	uint length;
} JZData;

JZData *JZDataReplaceInRange(JZData *self, JZRange *range, Byte *buf);
JZData *JZDataAdd(JZData *self, Byte *buf, uint length);

void JZDataFree(JZData *data);

JZData *JZDataCreatWithNumber(int number);
JZData *JZDataCreatWithLength(uint length);
JZData *JZDataCreat(Byte *buf, uint length);

// ================== JZDictionary ======================
typedef struct dictionary {
	ushort key;
	void *value;	
	struct dictionary *nextDict;
} Dictionary;

typedef struct {
	uint count;
	Dictionary *headDict;
} JZDictionary;

typedef struct {
	Dictionary *lastDict;
	Dictionary *currentDict;
} DictionaryLookup;

Dictionary *JZDictionaryTakeOffForIndex(JZDictionary *self, uint index);
void *JZDictionaryValueForIndex(JZDictionary *self, uint index);
DictionaryLookup *JZDictionaryKeyLookup(JZDictionary *self, ushort key);

void *JZDictionaryValueForKey(JZDictionary *self, ushort key);
Dictionary *JZDictionaryAdd(JZDictionary *self, ushort key, void *value);
JZDictionary *JZDictionaryCreat(void);

void DictionaryFreeForDict(Dictionary *self, JZDictionaryValueType valueType);
void DictionaryFreeForKey(JZDictionary *self, ushort key, JZDictionaryValueType valueType);
void JZDictionaryFreeAll(JZDictionary *dictionary, JZDictionaryValueType valueType);

// ================== 其他 ======================
#define nullHandler(var, handler) 	do {					\
										if (var == NULL) {	\
									        goto handler;	\
									    }					\
									} while(0);				

#define JZPrintf(format,...) printf(format"\n",##__VA_ARGS__) 


#endif /* typedef_h */





