//
//  ViewController.m
//  Study_YYKit
//
//  Created by xiaowei on 16/3/18.
//  Copyright © 2016年 qiangxinyu. All rights reserved.
//

#import "ViewController.h"
#import <YYKit/YYKit.h>

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    
    YYLabel * label = [[YYLabel alloc]initWithFrame:self.view.bounds];
    [self.view addSubview:label];
    
    
    NSMutableAttributedString * attString = [[NSMutableAttributedString alloc]initWithString:@"测试  测试"];
    label.font = [UIFont systemFontOfSize:13];
    attString.backgroundColor = [UIColor orangeColor];
    attString.color = [UIColor purpleColor];
    
    label.attributedText = attString;
    
    
//    label.text = @"asdasd";
    
    
    NSLog(@"%@----",label.text);
    NSLog(@"color = %@",label.textColor);
    
    
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
