//
//  motDecoder.c
//  JZMot-C
//
//  Created by Jonor on 16/7/13.
//  Copyright © 2016年 Jonor. All rights reserved.
//

/** 
 * in -> [                         motDcoder                           ] -> out
 * in -> [ motData -> checkData -> motGroup -> motSegment -> motObject ] -> out
 */
#include "typedef.h"
#include "motData.h"
#include "motDecoder.h"
#include "motGroup.h"
#include "motSegment.h"
#include "motObject.h"

#include "app_config.h"

//======================== 声明／宏／类型 ==================================================================================
#define MaxAvailableMemory (18*1024) // 18k

 struct _privateMember {
    JZMotData *motData;
    ushort currentTansportID;
};

//======================== 变量 ==================================================================================
JZMotDecoder *motDecoder = NULL;


//======================== 函数 ==================================================================================

void processData(JZMotDecoder *self, char *buf, uint len)
{       
    // 判断可用内存是否足够
    if (MaxAvailableMemory - JZMemoryUsedTotal() < 2048) {
        JZPrintf("Lack of memory to process MOT data, will ignore.");
        return;
    }
    // 解析MOT信息头
    JZMotGroup *motGroup = JZMotGroupCreat(buf, len);
    if (motGroup == NULL) {
        return;
    }
    // 组合
    if (motGroup->motGroupType != MotGroupTypesMotDirectory) {
        // 提取segment
        JZMotSegment *motSegment = JZMotSegmentCreat(motGroup);
        if (motSegment == NULL) {
            JZMotGroupFree(motGroup);
            return;
        }
        // 按key查找
        JZMotObject *motObject = (JZMotObject *)JZDictionaryValueForKey(self->motObjectsTransportIdLookup, motGroup->TransportId);
        if (motObject == NULL) {
            //打印motData
            JZPrintf("Memory total:%d use:%d free:%d", JZMemoryUsedTotal()+OSMMMIoCtl(MMM_IOCTL_GETSZ_INALL, 0), JZMemoryUsedTotal(), OSMMMIoCtl(MMM_IOCTL_GETSZ_INALL, 0));
            JZPrintf("TransportId:%d", motGroup->TransportId);
            //JZMotDataPrintf(self->motData);
            // 复位motData
//            JZMotDataFree(self->motData);
//            self->motData = JZMotDataCreat();
//            
           // 删掉旧图motObject
            Dictionary *tranIdDict = JZDictionaryTakeOffForIndex(self->motObjectsTransportIdLookup, 0);
            DictionaryFreeForDict(tranIdDict, JZDictionaryValueTypeJZMotObject);

           // 创建新图motObject
            motObject = JZMotObjectCreat(motGroup->TransportId);
            if (motObject == NULL) {
                goto ERROR_HENDLER;
            }
            JZDictionaryAdd(self->motObjectsTransportIdLookup, motGroup->TransportId, motObject);            
        }
        // 添加
        JZMotObjectAddSegment(motObject, motSegment);
        if (gFlagSaveCurrentMotBuffer == YES) {
            gFlagSaveCurrentMotBuffer = NO;
            // 保存当前的原数据，BLE将会发送这个数据。
            JZMotDataAdd(self->motData, buf, len, motGroup->MOTSegmentNumber, motGroup->TransportId);            
            self->currentTansportID = motGroup->TransportId;
        }
ERROR_HENDLER:
        // 分析用的临时对象,用完释放
        JZMotSegmentFree(motSegment);
    }
    // 分析用的临时对象,用完释放
    JZMotGroupFree(motGroup);
    return; 
}

JZData *getMotData(JZMotDecoder *self) {
    if (self->currentTansportID == 0) return NULL;

    JZData *data = JZMotDataOutput(self->motData, self->currentTansportID);
    //if (data == NULL) {
        self->currentTansportID = 0;
    //}
    return data;
}

JZMotDecoder *newMotDecoder(void) {
    memoryTableClear();
    JZMotDecoder *motDcoder = (JZMotDecoder *)JZAlloc(1, sizeof(JZMotDecoder));
    if (motDcoder == NULL) {
        JZPrintf("motDcoder creat failed.");
        return NULL;
    }
    
    motDcoder->motObjectsTransportIdLookup = JZDictionaryCreat();
    if (motDcoder->motObjectsTransportIdLookup == NULL) {
        goto ERROR_HENDLER;
    }
    
    motDcoder->motData = JZMotDataCreat();
    if (motDcoder->motData == NULL) {
        goto ERROR_HENDLER;
    }
    
    motDcoder->processData = processData;
    motDcoder->getMotData = getMotData;
    return motDcoder;
ERROR_HENDLER:
    freeMotDecoder(motDcoder);
    return NULL;
}

void freeMotDecoder(JZMotDecoder *self) {
    if (self == NULL) {
        return;
    }
    self->processData = NULL;
    self->getMotData = NULL;
    JZDictionaryFreeAll(self->motObjectsTransportIdLookup, JZDictionaryValueTypeJZMotObject);
    JZMotDataFree(self->motData);
    JZFree(self);
    self = NULL;
}




