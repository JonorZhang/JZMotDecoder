//
//  motData.c
//  JZMot-C
//
//  Created by Jonor on 16/7/13.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#include "motData.h"

JZData *JZMotDataOutput(JZMotData *self, ushort transportId) {
    if (self == NULL) return NULL;
	
    // 释放掉上一个已经发送的字典的内存
    static Dictionary *dictToFree = NULL;
    if (dictToFree != NULL) {
        DictionaryFreeForDict(dictToFree, JZDictionaryValueTypeJZData);
        dictToFree = NULL;        
    }

    // 查找大字典里有没有这个transportId
    DictionaryLookup *dictLookup = JZDictionaryKeyLookup(self->tranIdDict, transportId); 
	if (dictLookup == NULL || dictLookup->currentDict == NULL) {
        goto ERROR_HENDLER;
    }
	
	JZDictionary *segNbrDict = (JZDictionary *)dictLookup->currentDict->value;
	if (segNbrDict == NULL) {
        goto ERROR_HENDLER;
    }
	
	// Dictionary *currentDict = segNbrDict->headDict->nextDict;
	Dictionary *currentDict = JZDictionaryTakeOffForIndex(segNbrDict, 0);
	if (currentDict == NULL) {
        goto ERROR_HENDLER;
    }
    
    // 记录将要释放的字典
	dictToFree = currentDict;
    
    // 获取字典的value(MOT原数据)
	JZData *data = (JZData *)currentDict->value;
	return data;

ERROR_HENDLER:
    JZPrintf("JZMotDataOutput ERROR_HENDLER!");
//	// 释放segNbrDict头
//	JZDictionaryFreeAll(segNbrDict, JZDictionaryValueTypeJZData);
//	// 释放tranIdDict
//	DictionaryFreeForKey(self->tranIdDict, transportId, JZDictionaryValueTypeJZData); //这个value其实是 JZDictionary*
	return NULL;
}


JZDictionary *JZMotDataAdd(JZMotData *self, Byte *buf, uint length, ushort segmentNumber, ushort transportId) {
	if (self == NULL || self->tranIdDict == NULL || buf == NULL || length == 0) return NULL;

//	if (self->tranIdDict->count >= 3) {
//        Dictionary *tranIdDict = (Dictionary *)JZDictionaryTakeOffForIndex(self->tranIdDict, 0);
//        JZDictionaryFreeAll((JZDictionary *)tranIdDict->value, JZDictionaryValueTypeJZData);
//        JZFree(tranIdDict);
//        self->tranIdDict->count--;
//	}

    // 大字典里查找有没有ID
    Dictionary *currentDict = NULL;
    DictionaryLookup *dictLookup = JZDictionaryKeyLookup(self->tranIdDict, transportId);
    // 没有这个ID的字典，创建它
    if (dictLookup ==  NULL) {
    	JZDictionary *segNbrDict = JZDictionaryCreat(); // JZDictionary<segNbr, JZData *>
        JZPrintf("hhhh4");
    	currentDict = JZDictionaryAdd(self->tranIdDict, transportId, segNbrDict); // JZDictionary<tranId, JZDictionary<segNbr, JZData *> *>
    } else {
        currentDict = dictLookup->currentDict;
    }
    if (currentDict ==  NULL) {
        return NULL;
    }
    
    // 获取字典位置
    JZDictionary *segNbrDict = (JZDictionary *)currentDict->value;
    if (segNbrDict == NULL) {
        JZPrintf("hhhh5");
        return NULL;
    }
    // 创建value
	JZData *value = JZDataCreat(buf, length);
    // 添加value
    JZDictionaryAdd(segNbrDict, segmentNumber, value);

	return segNbrDict;
}

void JZMotDataFree(JZMotData *self) {
    if (self == NULL || self->tranIdDict != NULL) {
        return;
    }        
    JZPrintf("sss");

// 从头部拿出一个字典(最先放进去的字典)
    Dictionary *currentDict = (Dictionary *)self->tranIdDict->headDict->nextDict;
    for ( ;  currentDict != NULL; ) {
        JZDictionaryFreeAll((JZDictionary *)currentDict->value, JZDictionaryValueTypeJZData);
        JZPrintf("eee");
        currentDict = currentDict->nextDict;
    }
    JZPrintf("rrr");

    JZDictionaryFreeAll(self->tranIdDict, JZDictionaryValueTypeJZData);
    JZPrintf("ttt");
    JZFree(self);
    JZPrintf("yyy");
    self = NULL;
}

JZMotData *JZMotDataCreat(void) {
	JZMotData *jzMotData = (JZMotData *)JZAlloc(1, sizeof(JZMotData));
	if (jzMotData == NULL) {
		return NULL;
	}
	jzMotData->tranIdDict = JZDictionaryCreat();
	if (jzMotData->tranIdDict == NULL) {
        goto ERROR_HENDLER;
    }
    return jzMotData;

ERROR_HENDLER:
	JZMotDataFree(jzMotData);
    return NULL;
}

void JZMotDataPrintf(JZMotData *self) {
    if (self == NULL || self->tranIdDict == NULL) return;
    Dictionary *tranIdDict = self->tranIdDict->headDict->nextDict; // JZDictionary<id, MotData *>
    for ( ; tranIdDict != NULL && tranIdDict->value != NULL; ) {
        Dictionary *dataDict = ((JZDictionary *)(tranIdDict->value))->headDict->nextDict;
        for ( ; dataDict != NULL && dataDict->value != NULL; ) {
            Byte *bytes = ((JZData *)dataDict->value)->bytes;
            uint len = ((JZData *)dataDict->value)->length;
            JZPrintf("[%02d]:[%02x %02x]...[%02x %02x]    Len:%d", dataDict->key, bytes[0], bytes[1], bytes[len-1], bytes[len-2], len);
            dataDict = dataDict->nextDict;
        }
        tranIdDict = tranIdDict->nextDict;
    }
}

