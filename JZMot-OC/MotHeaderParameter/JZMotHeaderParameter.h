//
//  JZMotHeaderParameter.h
//  mot
//
//  Created by Jonor on 16/7/7.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef enum {
    // Reserved for MOT Protocol Extensions = 0x00,
    MOTParameterTypePermitOutDatedVersions = 0x01,
    MOTParameterTypeCreationTime = 0x02,    // Reserved for MOT Protocol Extensions
    MOTParameterTypeStartValidity = 0x03,   // Reserved for MOT Protocol Extensions
    MOTParameterTypeExpireTime = 0x04,      // Reserved for MOT Protocol Extensions (Slideshow)
    MOTParameterTypeTriggerTime = 0x05,     // Also in  MOT Slideshow Spec
    MOTParameterTypeVersionNumber = 0x06,   // Reserved for MOT Protocol Extensions
    MOTParameterTypeRetransmissionDistance = 0x07,
    MOTParameterTypeGroupReference = 0x08,  // Reserved for MOT Protocol Extensions
    MOTParameterTypeExpiration = 0x09,
    MOTParameterTypePriority = 0x0A,
    MOTParameterTypeLabel = 0x0B,
    MOTParameterTypeContentName = 0x0C,     // Also in  MOT Slideshow Spec
    MOTParameterTypeUniqueBodyVersion = 0x0D,
    // Reserved for MOT Protocol Extensions = 0x0E,
    MOTParameterTypeContentDescription = 0x0F, // Reserved for MOT Protocol Extensions
    MOTParameterTypeMimeType = 0x10,
    MOTParameterTypeCompressionType = 0x11,
    // Reserved for MOT Protocol Extensions = 0x12 - 0x1F,
    MOTParameterTypeAdditionalHeader = 0x20,
    MOTParameterTypeProfileSubset = 0x21,
    // Reserver for MOT Protocol Extensions = 0x22,
    MOTParameterTypeCAInfo = 0x23,
    MOTParameterTypeCAReplacementObject = 0x24,
    // Application Specific = 0x25 - 0x3F,
    MOTParameterTypeCategory_SlideID = 0x25,        // MOT Slideshow Specific
    MOTParameterTypeCategoryTitle = 0x26,           // MOT Slideshow Specific
    MOTParameterTypeClickThroughURL = 0x27,         // MOT Slideshow Specific
    MOTParameterTypeAlternativeLocationURL = 0x28,  // MOT Slideshow Specific
    MOTParameterTypeAlert = 0x29,                   // MOT Slideshow Specific
    MOTParameterTypeApplicationSpecific = 0x3F
} MOTParameterType;


@interface JZMotHeaderParameter : NSObject

@property (nonatomic, assign) Byte PLI;
@property (nonatomic, assign) Byte ParameterId;

@property (nonatomic, strong) NSMutableData *ParameterByteData;


+ (instancetype)headerParameter;
- (instancetype)ProcessMOTHeaderParameter:(int)startIndex data:(NSData *)byteData outIndex:(int *)nextIndex;

@end
