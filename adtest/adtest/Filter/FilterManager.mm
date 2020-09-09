 //
//  FilterManager.m
//  adtest
//
//  Created by Zhi Zhuang on 2020/8/25.
//  Copyright © 2020 Zhi Zhuang. All rights reserved.
//

#import "FilterManager.h"

@implementation FilterManager{
    NSMutableDictionary * wordsDic;
}

-(void)loadWord{
    NSString *wordFilePath = [[NSBundle mainBundle] pathForResource:@"ShieldSDK.bundle/shieldwords.txt" ofType:nil];
    
    NSString *filterStrings = [NSString stringWithContentsOfFile:wordFilePath
                                                        encoding:NSUTF8StringEncoding
                                                           error:nil];
    
    NSArray *filters = [filterStrings componentsSeparatedByString:@"\n"];
    wordsDic = [NSMutableDictionary dictionary];
    NSString * str;
    for (int n = 0; n < filters.count ; n++) {
        str = filters[n];
        NSMutableArray * arr =wordsDic[[str substringToIndex:1]];
        if( nil != arr){
            [arr addObject:@[[NSNumber numberWithInteger:str.length],str]];
        }else{
            [wordsDic setValue:[NSMutableArray arrayWithObject:@[[NSNumber numberWithInteger:str.length],str]] forKey:[str substringToIndex:1]];
        }
    }
}

-(NSMutableString* )check:(NSString*) word{
    NSMutableString * orginStr = [word mutableCopy];
    NSString * str = [word lowercaseString];
    int len = (int)str.length;
    NSMutableArray * special = [self getValidCharacters:word];
    for (int i = 0;  i< len ; i++) {
        NSString * first = [str substringWithRange:NSMakeRange(i, 1)];
        NSMutableArray* arr = [wordsDic objectForKey:first];
        if (arr && arr.count > 0) {
            for (int j = 0; j < (int)arr.count ; j++) {
                NSArray * child = arr[j];
                int num = [child[0] intValue];
                if((i+num) <= len){
                    NSString * fragment = @"";
                    int hitCount = 0;
                    if (special.count == 0) {
                        //无特殊字符
                        fragment = [str substringWithRange:NSMakeRange(i, num)];
                    }else{
                        //有特殊字符
                        int specialCount = (int)special.count - 1;
                        int lastIndex = specialCount;
                        for (int n = specialCount; n >= 0; n--) {
                            int beginIndex = i + num;
                            int specialIndex = [special[n] intValue];
                            if (i > specialIndex) {
                                lastIndex = specialIndex;
                                specialCount--;
                                [special removeObjectAtIndex:n];
                                continue;
                            }
                            if (beginIndex > (specialIndex-hitCount) ) {
                                hitCount++;
                                if (n == specialCount) {
                                    fragment = [fragment stringByAppendingString:[str substringWithRange:NSMakeRange(i, specialIndex - i)]];
                                }else {
                                    fragment = [fragment stringByAppendingString:[str substringWithRange:NSMakeRange(lastIndex+1, specialIndex - lastIndex-1)]];
                                }
                                lastIndex = specialIndex;
                                //解决最后一个特殊字符的问题
                                if(n==0){
                                    fragment = [fragment stringByAppendingString:[str substringWithRange:NSMakeRange(lastIndex+1, i+num - lastIndex+hitCount-1)]];
                                }
                            }else{
                                if (hitCount>0) {
                                    fragment = [fragment stringByAppendingString:[str substringWithRange:NSMakeRange(lastIndex+1, i+num - lastIndex+hitCount-1)]];
                                }else{
                                    fragment = [str substringWithRange:NSMakeRange(i, num)];
                                }
                                break;
                            }
                        }
                    }
                    if ([fragment isEqualToString:child[1]]) {
                        [orginStr replaceCharactersInRange:NSMakeRange(i, num+hitCount) withString:createStars(num+hitCount)];
                        i = i+num-1;
                        hitCount = 0;
                        break;
                    }
                }
            }
        }
    }
    return orginStr;
}

NSString *(^createStars)(NSInteger) = ^(NSInteger length){
    NSMutableString *stars = [NSMutableString string];
    for (int i=0;i<length;i++) {
        [stars appendString:@"*"];
    }
    return stars.copy;
};

// 是否合法字符
- (NSMutableArray * )getValidCharacters:(NSString *)character
{
    if (character.length==0 || !character) {
        return NULL;
    }
    NSError *error = nil;
    NSString *pattern = @"[^\uff10-\uff19\\u0030-\\u0039\uff41-\uff5a\\u0061-\\u007a\uff21-\uff3a\\u0041-\\u005a\u4e00-\u9fa5\u0f00-\u0fff\\u002b]";//正则取反
    NSRegularExpression *regularExpress = [NSRegularExpression regularExpressionWithPattern:pattern options:NSRegularExpressionCaseInsensitive error:&error];//这个正则可以去掉所有特殊字符和标点
    NSMutableArray * resultArr = [NSMutableArray array];
    [regularExpress enumerateMatchesInString:character options:0 range:NSMakeRange(0, [character length]) usingBlock:^(NSTextCheckingResult * _Nullable result, NSMatchingFlags flags, BOOL * _Nonnull stop) {
        [resultArr insertObject:[NSNumber numberWithUnsignedInteger:result.range.location] atIndex:0];
    }];
    return resultArr;
}

#pragma mark - 文件夹操作
- (NSString *)rootDir
{
    if (!_rootDir) {
        
        NSString *documentDirectory = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
        _rootDir = [[documentDirectory stringByAppendingPathComponent:@"ShieldSDK"] copy];
        
        NSFileManager *fm = [NSFileManager defaultManager];
        if (![fm fileExistsAtPath:_rootDir] &&
            ![fm createDirectoryAtPath:_rootDir
           withIntermediateDirectories:YES
                            attributes:nil
                                 error:nil]) {
            NSLog(@"***文件夹创建失败，%@", _rootDir);
        }
    }
    
    return _rootDir;
}

@end
