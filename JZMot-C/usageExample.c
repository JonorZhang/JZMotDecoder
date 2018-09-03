////
////  usageExample.c
////  JZMot-C
////
////  Created by Jonor on 16/7/13.
////  Copyright © 2016年 Jonor. All rights reserved.
////

//// 1. 包含头文件
//#include "JZMot.h"

//// ========================================================================================
//// 2. 声明解码器
//JZMotDecoder *motDecoder;

//// ========================================================================================
//void SysInit()
//{
//	// 3. 创建解码器
//	if (motDecoder == NULL) {
//		motDecoder = newMotDecoder();
//	}
//}

//// ========================================================================================
//void updataServiceData()
//{
//	case SLS:
//	{
//		// 4. 调用解码器
//		if (motDecoder!= NULL) {
//			motDecoder->processData(motDecoder, buf, len); // 处理
//		}
//	}	
//}

//// ========================================================================================
//void bleSendData(void)
//{
//	switch (MotDecoderGenerateDataType) {
//	case MotDataWithSegments: // MOT格式的分组数据(接收到的原始数据)
//		// 5. 获取数据, 无数据返回NULL。
//		JZData *data = motDecoder->getMotData(motDecoder);
//		if (data != NULL) {
//			sendData(data->bytes, data->length);
//		}
//		break;
//	case ImageDataWithSegments: // 图片格式的分组数据
//		break;
//	case ImageDataWithIntegrated: // 图片格式的完整数据
//		break;
//	}
//}


//// ========================================================================================
//void SysExit()
//{
//	// 6. 销毁解码器
//	if (motDecoder!= NULL) {
//		freeMotDecoder(motDecoder);
//		motDecoder = NULL;
//	}
//}
