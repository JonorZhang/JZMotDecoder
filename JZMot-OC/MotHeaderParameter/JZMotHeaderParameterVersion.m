//
//  JZMotHeaderParameterVersion.m
//  mot
//
//  Created by Jonor on 16/7/6.
//  Copyright © 2016年 Jonor. All rights reserved.
//

#import "JZMotHeaderParameterVersion.h"

@implementation JZMotHeaderParameterVersion

+ (instancetype)version {
    JZMotHeaderParameterVersion *version = [[self alloc] init];
    return version;
}

- (void)ProcessData
{
    Byte *ParameterByteDataPtr = (Byte *)self.ParameterByteData.bytes;
    //base.ProcessData();

    // Make sure there is enough data to process.
    if (self.ParameterByteData.length < 1)
        NSLog(@"Not enough data associated with the parameter to process the version.");
    
    // See if the time is now.
    self.version = ParameterByteDataPtr[0];
}
@end
