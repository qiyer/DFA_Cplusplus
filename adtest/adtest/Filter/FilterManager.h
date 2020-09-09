//
//  FilterManager.h
//  adtest
//
//  Created by Zhi Zhuang on 2020/8/25.
//  Copyright © 2020 Zhi Zhuang. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface FilterManager : NSObject

@property (nonatomic, copy  ) NSString *rootDir;// 根目录文件夹路径

-(void)loadWord;
-(NSMutableString* )check:(NSString*) word;
@end

NS_ASSUME_NONNULL_END
