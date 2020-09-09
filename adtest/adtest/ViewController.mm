//
//  ViewController.m
//  adtest
//
//  Created by Zhi Zhuang on 2020/7/30.
//  Copyright © 2020 Zhi Zhuang. All rights reserved.
//

#import "ViewController.h"
#import "FilterManager.h"
#include "DFilter.hpp"


@interface ViewController ()

@end

DFilter * dFilter;

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    NSString *wordFilePath = [[NSBundle mainBundle] pathForResource:@"ShieldSDK.bundle/shieldwords.txt" ofType:nil];
    
    NSLog(@"init-begin-time:%f",[NSDate date].timeIntervalSince1970);
    dFilter->load([wordFilePath UTF8String]);
    NSLog(@"init-end  -time:%f",[NSDate date].timeIntervalSince1970);
    std::string strs = "默克尔删掉中国茉莉花运动角色就低价位默罕默德觉得修改了AA您的密码SB，请谨记您的新密码。如非本人操作，则密码可能已经泄露，建议修改密码,藏暴乱ww手机丢失菊色宫，气秃，江，泽 民 毛泽、东默克尔删掉中国茉莉花运动角色就低价位默cao你大爷";
    std::string resultStr = dFilter->find(strs);
    NSLog(@"check-end -time:%f",[NSDate date].timeIntervalSince1970);
    NSLog(@"%@",[NSString stringWithUTF8String:resultStr.c_str()]);
    
    NSLog(@"=======================================================================");
    
    NSLog(@"init-begin-time:%f",[NSDate date].timeIntervalSince1970);
    FilterManager * fm = [FilterManager new];
    [fm loadWord];
    NSLog(@"init-end  -time:%f",[NSDate date].timeIntervalSince1970);
    NSMutableString * result = [fm check:@"默克尔删掉中国茉莉花运动角色就低价位默罕默德觉得修改了AA您的密码SB，请谨记您的新密码。如非本人操作，则密码可能已经泄露，建议修改密码,藏暴乱ww手机丢失菊色宫，气秃，江，泽 民 毛泽、东默克尔删掉中国茉莉花运动角色就低价位默cao你大爷"];
    NSLog(@"check-end -time:%f",[NSDate date].timeIntervalSince1970);
    NSLog(@"result:%@",result);
    
}

-(void)d:(NSString *) format,...NS_REQUIRES_NIL_TERMINATION{
    va_list args;
    va_start(args, format);
    NSLogv(format, args);
    if (format)
       {
           NSString *otherString;
           while ((otherString = va_arg(args, NSString *)))
           {
               //依次取得所有参数
               NSLog(@"args:%@",otherString);
           }
       }
    va_end(args);
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
