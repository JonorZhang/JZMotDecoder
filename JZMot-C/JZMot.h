//
//  JZMot.h
//  JZMot-C
//
//  Created by Jonor on 16/7/13.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#ifndef JZMot_h
#define JZMot_h

#include "typedef.h"
#include "motData.h"
#include "motCheck.h"
#include "motGroup.h"
#include "motObject.h"
#include "motSegment.h"
#include "motDecoder.h"


// 配置经过解码器之后要生成的数据形式

// MOT格式的分组数据(接收到的原始数据)
#define MotDataWithSegments     0//default
// 图片格式的分组数据
#define ImageDataWithSegments   1
// 图片格式的完整数据
#define ImageDataWithIntegrated 2

#define MotDecoderGenerateDataType MotDataWithSegments //default

#endif /* JZMot_h */
