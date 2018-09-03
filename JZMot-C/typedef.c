//
//  typedef.c
//  JZMot-C
//
//  Created by Jonor on 16/7/13.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#include "typedef.h"

#include "motData.h"
#include "motGroup.h"
#include "motObject.h"
#include "motSegment.h"

#include "app_config.h"

// ===================== 动态内存 =============================================

typedef struct {
	void *addr;
	uint size;
} __Memory;
#define MaxMemoryTable 125
__Memory __memoryTable__[MaxMemoryTable];
uint memoryUsedTotal = 0;

void memoryTableClear(void) {
	memset(__memoryTable__, 0, sizeof(__memoryTable__));
	memoryUsedTotal = 0;
}

uint JZMemoryUsedTotal(void) {
	return memoryUsedTotal;
}

void *JZAlloc(uint n, uint size) {
	//void *p = calloc(n, size);
//    JZPrintf("Memory total:%d use:%d free:%d alloc:%d", JZMemoryUsedTotal()+OSMMMIoCtl(MMM_IOCTL_GETSZ_INALL, 0), JZMemoryUsedTotal(), OSMMMIoCtl(MMM_IOCTL_GETSZ_INALL, 0), size);
	void *p = APP_MMM_MALLOC(size, MMM_ALLOC_NORMAL);
	if (p == NULL) {
		JZPrintf("JZAlloc failed!");
		return NULL;
	}
	memset(p, 0, size);
	memoryUsedTotal += size;
	uint index;
	for (index = 0; index < MaxMemoryTable; ++index) {
		if (__memoryTable__[index].addr == NULL) {
			__memoryTable__[index].addr = p;
			__memoryTable__[index].size = size;
			break;
		}
	}
	if (index >= MaxMemoryTable) {
		JZPrintf("MaxMemoryTable too small.");
	}
	return p;
}

void *JZRealloc(void *p, uint size) {
	if (p == NULL) return NULL;
	// void *newBytes = realloc(p, size);
	// JZPrintf("Memory total:%d use:%d free:%d alloc:%d", JZMemoryUsedTotal()+OSMMMIoCtl(MMM_IOCTL_GETSZ_INALL, 0), JZMemoryUsedTotal(), OSMMMIoCtl(MMM_IOCTL_GETSZ_INALL, 0), size);
	void *newBytes = APP_MMM_REALLOC(p, size, MMM_ALLOC_NORMAL);
	if (newBytes == NULL) {
		JZPrintf("JZAlloc failed!");
		return NULL;
	}
	uint index;
	for (index = 0; index < MaxMemoryTable; ++index) {
		if (__memoryTable__[index].addr == p) {
			__memoryTable__[index].addr = newBytes;
			memoryUsedTotal -= __memoryTable__[index].size;
			memoryUsedTotal += size;
			__memoryTable__[index].size = size;
			break;
		}
	}
	if (index >= MaxMemoryTable) {
		JZPrintf("Can't match 'p' in __memoryTable__.");
		for (index = 0; index < MaxMemoryTable; ++index) {
			if (__memoryTable__[index].addr == NULL) {
				__memoryTable__[index].addr = newBytes;
				__memoryTable__[index].size = size;
				memoryUsedTotal += size;
				break;
			}
		}
	}

	return newBytes;
}
void JZFree(void *p) {
	if (p == NULL) return;
	uint index;
	for (index = 0; index < MaxMemoryTable; ++index) {
		if (__memoryTable__[index].addr == p) {
			__memoryTable__[index].addr = NULL;
			memoryUsedTotal -= __memoryTable__[index].size;
			__memoryTable__[index].size = 0;
        	// free(p);
			APP_MMM_FREE(p);
			p = NULL;	
			break;
		}
	}
	if (index >= MaxMemoryTable) {
		JZPrintf("Can't match 'p' in __memoryTable__.");
	}
}

// ===================== JZData =============================================

JZData *JZDataCreatWithNumber(int number) {
	Byte bytes[10] = {0};
	uint length = sprintf(bytes, "%d", number);
	JZData *data = JZDataCreat(bytes, length+1);
	return data;
}

JZData *JZDataReplaceInRange(JZData *self, JZRange *range, Byte *buf) {
	if (self == NULL) {
		return self;
	}
	if (self->length < range->index + range->length) {
		return self;
	}
	memcpy(self->bytes + range->index, buf, range->length);
	return self;
}

JZData *JZDataAdd(JZData *self, Byte *buf, uint length) {
	if (self == NULL) {
		self = JZDataCreat(buf, length);
		return self;
	}
	if (buf == NULL || length == 0) {
		return self;
	}

	if (self->bytes == NULL) {
		self->bytes = (Byte *)JZAlloc(1, length);
		if (self->bytes == NULL) {
			goto ERROR_HENDLER;
		}
		//申请内存成功
		self->length = length;
		memcpy(self->bytes, buf, length);
		return self; 
	} else {
		uint newLength = self->length + length;
		Byte *newBytes = (Byte *)JZRealloc(self->bytes, newLength);
		if (newBytes == NULL) {
			goto ERROR_HENDLER;
		}
		self->bytes = newBytes;
		memcpy((self->bytes + self->length), buf, length);
		self->length = newLength;
		return self;
	}

	ERROR_HENDLER:
	JZDataFree(self);
	return NULL;
}

void JZDataFree(JZData *self) {
	if (self == NULL) return;
	if (self->bytes != NULL) {
		JZFree(self->bytes);
	}
	JZFree(self);
	self = NULL;
}

JZData *JZDataCreat(Byte *buf, uint length) {
	JZData *data = (JZData *)JZAlloc(1, sizeof(JZData));
	if (data != NULL) {	
		if (length == 0) {
			return data; 
		} else {
			data->bytes = (Byte *)JZAlloc(1, length);
			if (data->bytes != NULL) {
				//申请内存成功
				data->length = length;
				if (buf != NULL) {
					memcpy(data->bytes, buf, length);
				}
				return data; 
			}
		}
	}
	JZDataFree(data);
	return NULL;
}

JZData *JZDataCreatWithLength(uint length) {
	return JZDataCreat(NULL, length);
}

// ===================== JZDictionary =============================================

void DictionaryFreeForDict(Dictionary *self, JZDictionaryValueType valueType) {
//    JZPrintf("DictionaryFreeForDict");
	if (self == NULL) {
		return;
	}

	// 由于原本是void ＊value
	// 不知道内部结构，因此必须强制换成相应结构类型。
	switch (valueType) {
	case JZDictionaryValueTypeJZData:
		JZDataFree((JZData *)self->value);
		break;
	case JZDictionaryValueTypeJZMotData:
		JZMotDataFree((JZMotData *)self->value);
		break;
	case JZDictionaryValueTypeJZMotObject:
		JZMotObjectFree((JZMotObject *)self->value);
		break;
	case JZDictionaryValueTypeJZMotGroup:
		JZMotGroupFree((JZMotGroup *)self->value);
		break;
	case JZDictionaryValueTypeJZMotSegment:
		JZMotSegmentFree((JZMotSegment *)self->value);
		break;
	default:
		break;
	}

	JZFree(self);
	self = NULL;
}

Dictionary *DictionaryTialLookup(JZDictionary *self) {
//	JZPrintf("DictionaryTialLookup");
	if (self == NULL) {
		JZPrintf("self == NULL");
		return NULL;
	}
//    JZPrintf("DictionaryTialLookup 0");
	Dictionary *currentDict = self->headDict;
	if (currentDict == NULL) {
		JZPrintf("currentDict == NULL");
	}
//    JZPrintf("DictionaryTialLookup 1");
	while (currentDict->nextDict != NULL) {
//        JZPrintf("DictionaryTialLookup loop");
		currentDict = currentDict->nextDict;
	}
//    JZPrintf("DictionaryTialLookup exit");
	return currentDict;
}

// 这个函数会从self卸载下标为index的字典，并将它的前一个字典指向它的后一个字典，返回这个字典，注意:不会清掉这个字典的内存。
Dictionary *JZDictionaryTakeOffForIndex(JZDictionary *self, uint index) {
//    JZPrintf("JZDictionaryTakeOffForIndex");
	if (self == NULL || self->headDict == NULL || self->count == 0 || index >= self->count) {
		return NULL;
	}

	Dictionary *currentDict = self->headDict->nextDict;
	Dictionary *lastDict = self->headDict;
	
	for (int i = 0; i < index; ++i) {
		if (currentDict == NULL) {
			JZPrintf("JZDict err!");
			return NULL;
		}
		lastDict = currentDict;
		currentDict = currentDict->nextDict;
	}
	
	lastDict->nextDict = currentDict->nextDict;
	self->count--;

	return currentDict;
}

void *JZDictionaryValueForIndex(JZDictionary *self, uint index) {
//    JZPrintf("JZDictionaryValueForIndex");
	if (self == NULL || self->count == 0 || index >= self->count) {
		return NULL;
	}
	Dictionary *currentDict = self->headDict->nextDict;
	for (int i = 0; i < index; ++i) {
		currentDict = currentDict->nextDict;
	}
	return currentDict->value;
}

DictionaryLookup *JZDictionaryKeyLookup(JZDictionary *self, ushort key) {
//    JZPrintf("JZDictionaryKeyLookup");
	if (self == NULL || self->count == 0) {
		return NULL;
	}
	Dictionary *currentDict = self->headDict->nextDict;
	Dictionary *lastDict = self->headDict;
	for ( ; currentDict != NULL; ) {
		if (currentDict->key == key) {
			static DictionaryLookup dictionaryLookup;
			dictionaryLookup.lastDict = lastDict;
			dictionaryLookup.currentDict = currentDict;
			return &dictionaryLookup;
		}
		lastDict = currentDict;
		currentDict = currentDict->nextDict;
	}
	return NULL;
}

void *JZDictionaryValueForKey(JZDictionary *self, ushort key) {
//    JZPrintf("JZDictionaryValueForKey");
	if (self == NULL || self->count == 0) {
		return NULL;
	}
	DictionaryLookup *dictionaryLookup = JZDictionaryKeyLookup(self, key);
	if (dictionaryLookup == NULL) {
		return NULL;
	}
	return dictionaryLookup->currentDict->value;
}

void DictionaryFreeForKey(JZDictionary *self, ushort key, JZDictionaryValueType valueType) {
//    JZPrintf("DictionaryFreeForKey");
	if (self == NULL || self->count == 0) {
		return;
	}
	DictionaryLookup *dictionaryLookup = JZDictionaryKeyLookup(self, key);
	if (dictionaryLookup == NULL) {
		return;
	}
	dictionaryLookup->lastDict->nextDict = dictionaryLookup->currentDict->nextDict;
	DictionaryFreeForDict(dictionaryLookup->currentDict, valueType);
	self->count --;		
}

Dictionary *JZDictionaryAdd(JZDictionary *self, ushort key, void *value) {
//    JZPrintf("JZDictionaryAdd");
	if (self == NULL) {
		return NULL;
	}
	
	Dictionary *newDictionary = (Dictionary *)JZAlloc(1, sizeof(Dictionary));
	if (newDictionary == NULL) {
		return NULL;
	}
	newDictionary->key = key;
	newDictionary->value = value;
	newDictionary->nextDict = NULL;

	Dictionary *dictionaryTail = DictionaryTialLookup(self);
	if (dictionaryTail == NULL) {
		JZPrintf("dictionaryTail == NULL");
	}
	dictionaryTail->nextDict = newDictionary;
	self->count ++;

	return newDictionary;
}

void JZDictionaryFreeAll(JZDictionary *self, JZDictionaryValueType valueType) {
//	JZPrintf("JZDictionaryFreeAll");
	if (self == NULL) {
		return;
	}
	Dictionary *currentDict = self->headDict;
	Dictionary *nextDict;
	for (; currentDict != NULL; ) {
		nextDict = currentDict->nextDict;
		DictionaryFreeForDict(currentDict, valueType);
		currentDict = nextDict;
	}
	JZFree(self);
	self = NULL;
}

JZDictionary *JZDictionaryCreat(void) {
//	JZPrintf("JZDictionaryCreat");
	JZDictionary *jzDictionary = (JZDictionary *)JZAlloc(1, sizeof(JZDictionary));
	if (jzDictionary == NULL) {
		return NULL;
	}
	jzDictionary->headDict = (Dictionary *)JZAlloc(1, sizeof(Dictionary));
	if (jzDictionary->headDict == NULL) {
		JZFree(jzDictionary);
		return NULL;
	}
	jzDictionary->count = 0;
	jzDictionary->headDict->key = 0;
	jzDictionary->headDict->value = NULL;
	jzDictionary->headDict->nextDict = NULL;

	return jzDictionary;
}





