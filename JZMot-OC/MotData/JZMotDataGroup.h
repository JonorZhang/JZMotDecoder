//
//  JZMotDataGroup.h
//  AppCast
//
//  Created by Jonor on 16/6/16.
//  Copyright © 2016年 soundmax. All rights reserved.
//

#import <Foundation/Foundation.h>


typedef enum {
    DataGroupTypesUnknown = 0xFF,
    DataGroupTypesMotHeaderInfo = 3,
    DataGroupTypesMotData = 4,
    DataGroupTypesMotDataAndCaParameters = 5,
    DataGroupTypesMotDirectory = 6
} DataGroupTypes;



@interface JZMotDataGroup : NSObject

@property (nonatomic, assign) BOOL ExtensionFlag;
@property (nonatomic, assign) BOOL CRCFlag;
@property (nonatomic, assign) BOOL SegmentFlag;
@property (nonatomic, assign) BOOL UserAccessFlag;
@property (nonatomic, assign) DataGroupTypes DataGroupType;
@property (nonatomic, assign) Byte ContinuityIndex;
@property (nonatomic, assign) Byte RepetitionIndex;
@property (nonatomic, assign) ushort ExtensionField;
@property (nonatomic, assign) BOOL SegmentLast;
@property (nonatomic, assign) ushort MOTSegmentNumber;
@property (nonatomic, assign) BOOL TransportIdFlag;
@property (nonatomic, assign) Byte UserAccessLength;
@property (nonatomic, assign) ushort TransportId;
@property (nonatomic, strong) NSMutableData * EndUserAddressField;
@property (nonatomic, assign) ushort CRC;
@property (nonatomic, strong) NSMutableData * DataGroupDataField;




+ (instancetype)dataGroup:(Byte *)groupData length:(NSInteger)len checkCRC:(BOOL)checkCRC;
- (BOOL)checkCRC:(Byte *)data length:(NSInteger)len crc:(ushort)crc;

@end
 