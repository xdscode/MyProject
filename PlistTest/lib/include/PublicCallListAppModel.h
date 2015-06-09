//
//  YouMiFeaturedAppModel.h
//  YouMiSDK
//
//  Created by Layne on 12-01-05.
//  Copyright (c) 2012年 YouMi Mobile Co. Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "publicHeader.h"

@interface PublicCallListAppModel : NSObject<NSCopying>

/*
 * 以下id类型属性的值保证不为nil，有可能为@""
 */

// 该开放源应用的标示
@property(nonatomic, retain, readonly)    NSString    *PublicCallstoreID;

// 应用名称
@property(nonatomic, retain, readonly)    NSString    *PublicCallname;

// 应用的类别
@property(nonatomic, retain, readonly)    NSString    *PublicCallcategory;

// 应用的详细描述
@property(nonatomic, retain, readonly)    NSString    *PublicCalldesc;

// 应用版权所有者
@property(nonatomic, retain, readonly)    NSString    *PublicCallauthor;

// 应用的大小
@property(nonatomic, retain, readonly)    NSString    *PublicCallsize;

// 应用的小图标
@property(nonatomic, retain, readonly)    NSString    *PublicCallsmallIconURL;

// 应用的大图标
@property(nonatomic, retain, readonly)    NSString    *PublicCalllargeIconURL;

// 简短广告词
@property(nonatomic, retain, readonly)    NSString    *PublicCalladText;

//提示语（例如：首次安装并创建新角色获取积分）
@property(nonatomic, retain, readonly)     NSString   *PublicCalltask_brief;

//步骤提示（例如：打开游戏创建新角色并体验3分钟 回到本应用即可获取{积分单位}）
@property(nonatomic, retain, readonly)     NSString   *PublicCalltask_steps;

// 过期时间
@property(nonatomic, retain, readonly)    NSDate      *PublicCallexpiredDate;

// 积分值[该值对有积分应用有效，无积分应用默认为0]。。已经完成的任务，points也会为0
@property(nonatomic, assign, readonly)    NSInteger   PublicCallpoints;

//appStoreID
@property(nonatomic, assign, readonly)     NSInteger    PublicCallappStoreID;

//appStoreURL
@property(nonatomic, retain, readonly)     NSString    *PublicCalllinkURL;

//bundleID
@property(nonatomic, retain, readonly)     NSString    *PublicCallidentifier;
@end
