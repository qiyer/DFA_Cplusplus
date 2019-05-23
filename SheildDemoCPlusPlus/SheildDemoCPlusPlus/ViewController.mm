//
//  ViewController.m
//  SheildDemoCPlusPlus
//
//  Created by Zhi Zhuang on 2019/4/11.
//  Copyright © 2019 Zhi Zhuang. All rights reserved.
//
#import "ViewController.h"
#include <vector>
#include "DFilter.hpp"

using namespace std;

#define NSEmptyValue(string) ((string==nil||[string isEqual:@""]||[string isEqual:[NSNull null]])?YES:NO)

@interface ViewController ()
@end

NSString *filePath;
DFilter dFilter;
NSArray *filters;

std::vector<std::wstring> cVector;

@implementation ViewController{
    UILabel *label;
}

- (void)viewDidLoad {
    [super viewDidLoad];

    label = [[UILabel alloc]initWithFrame:CGRectMake(20, 50, 330, 300)];
    [self.view addSubview:label];
    label.numberOfLines = 10;
    label.text = @"";
    
    [self log:@"init begin"];
    
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        [self loadFontURL];
        dispatch_sync(dispatch_get_main_queue(), ^{
            [self log:@"init --end"];


            [self log:@"search begin"];
            [self testFilter];
            [self log:@"search - end"];

        });
    });
    

}

-(void)log:(NSString*) text{
    NSDateFormatter * formatter = [[NSDateFormatter alloc ] init];
    [formatter setDateFormat:@"YYYY-MM-dd hh:mm:ss:SSS"];
    NSString *date =  [formatter stringFromDate:[NSDate date]];
    NSString *timeLocal = [[NSString alloc] initWithFormat:@"%@ :%@ \n",text, date];
    label.text = [label.text stringByAppendingString:timeLocal];;
}

-(void)testFilter{
    
    NSLog(@"结果：begin");
    std::string str ="1998,江泽民主席，满，黑火药是谁,,江泽民？0J2j 0 Fuck,国共 00 人民，国民党shi傻 逼";
    std::string result = dFilter.find(str);
    result.size();
    NSString * mystr = [NSString stringWithUTF8String:result.c_str()];
    NSLog(@"结果：%@",mystr);
}

-(void)loadFontURL
{
    filePath = [[NSBundle mainBundle] pathForResource:@"ShieldSDK.bundle/shieldwords.txt" ofType:nil];
    doLoadFont();
}

void doLoadFont(){
    NSString *filetrStrings = [NSString stringWithContentsOfFile:filePath
                                                        encoding:NSUTF8StringEncoding
                                                           error:nil];
    if (NSEmptyValue(filetrStrings)) {
        NSLog(@"***屏蔽词库读取失败");
        return;
    }

    filters = [filetrStrings componentsSeparatedByString:@","];
    NSLog(@"***屏蔽词库读取成功");
    vector<std::string> bVector;

    for(int i = 0; i < [filters count]; i++)
    {
        std::string  _value =  [[filters objectAtIndex:i] UTF8String];
        bVector.push_back(_value);
    }
    dFilter.load(bVector);
    bVector.clear();
}
@end
