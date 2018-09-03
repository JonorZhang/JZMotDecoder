//
//  JZMotDecoder.h
//  mot
//
//  Created by Jonor on 16/7/5.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#import <Foundation/Foundation.h>

// MotHeaderParameter
#import "JZMotHeaderParameter.h"
#import "JZMotHeaderParameterTime.h"
#import "JZMotHeaderParameterVersion.h"
#import "JZMotHeaderParameterText.h"

// MotObject
#import "JZMotObject.h"
#import "JZMotObjectHeader.h"
#import "JZMotObjectBody.h"

// MotData
#import "JZMotData.h"
#import "JZMotDataGroup.h"
#import "JZMotSegment.h"
#import "JZMotDirectory.h"

@class JZMotObject;

@interface JZMotDecoder : NSObject
// <string, MOTObject>
@property (nonatomic, strong) NSMutableDictionary *motObjectNameLookup;
// <ushort, MOTObject>
@property (nonatomic, strong) NSMutableDictionary<id, JZMotObject *> *motObjectsTransportIdLookup;


+ (instancetype)decoder;
- (JZMotObject *)ProcessDataGroups:(NSArray<NSData *> *)dataGroups CheckCRC:(BOOL)crc;
- (void)ProcessDataGroup:(NSData *)groupData CheckCRC:(BOOL)crc;

@end
