//
//  JZMotObjectHeader.h
//  mot
//
//  Created by Jonor on 16/7/6.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "JZMotHeaderParameter.h"

@class JZMotSegment;

typedef enum {
    MOTContentTypeGeneralData = 0,
    MOTContentTypeText  = 1,
    MOTContentTypeImage = 2,
    MOTContentTypeAudio = 3,
    MOTContentTypeVideo = 4,
    MOTContentTypeMotTransport = 5,
    MOTContentTypeSystem = 6,
    MOTContentTypeProprietaryTable = 0x3F
} MOTContentType;



@interface JZMotObjectHeader : NSObject

@property (nonatomic, assign) bool isHeaderComplete;
// The spec says that this a 28-bit unsigned binary number. "FFFFFFF" means "unknown size (at the beginning of the transmission).
@property (nonatomic, assign) uint BodySize;
@property (nonatomic, assign) Byte Version;
@property (nonatomic, assign) MOTContentType ContentType;
@property (nonatomic, strong) NSString *ContentName;
@property (nonatomic, assign) ushort ContentSubType;
@property (nonatomic, strong) NSString *MIMEType;
@property (nonatomic, strong) NSString *MIMESubType;


+ (instancetype)header;
- (void)addSegment:(JZMotSegment *)segment;
- (void)processSegments;
- (NSString *)getStatusText;
- (JZMotHeaderParameter *)getParameter:(MOTParameterType)parameterType;
- (NSArray<JZMotHeaderParameter *> *)getParameters;
- (void)UpdateParameters:(NSArray<JZMotHeaderParameter *> *)motHeaderParameters;

@end
