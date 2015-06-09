//
//  PublicCallList.h
//  YouMiSDK
//
//  Created by Layne on 12-01-05.
//  Copyright (c) 2012年 YouMi Mobile Co. Ltd. All rights reserved.
//


#import <Foundation/Foundation.h>
#import "PublicCallConfig.h"
#import "PublicCallListAppModel.h"
#import "PublicCallPointsManager.h"
#import "publicHeader.h"

@interface PublicCallList : NSObject
/*! 启动有米积分墙
 */
+ (void)PublicCallMyEnable;

/*! 显示积分墙
 * \param rewarded 是不是有积分模式
 * \param didShowBlockIn 成功显示积分墙时执行的block
 * \param didDismissBlockIn 关闭积分墙后执行的block
 * \returns 是否显示积分墙成功，积分墙显示不成功并不会调用didShowBlockIn和didDismissBlockIn
 */
+ (BOOL)PublicCallShowOffers:(BOOL)PublicRewarded PublicCallDidShowBlock:(void (^)(void))PublicCallDidShowBlock PublicDidDismissBlock:(void (^)(void))PublicDidDismissBlockIn;

/*! 获取积分墙的App广告列表
 * \param rewarded 是不是有积分模式
 * \param page 请求的数据第几页
 * \param count 每页有多少个广告（比如设置的requestPage为1，adCountPage为3.那么数据就可以看成是每页3个应用的形式在服务器上，每次请求服务器就把对应页的3个应用返回）
 * \param recievedBlock 接收广告列表的block, 其中NSArray的内容是 @[PublicCallListAppModel, PublicCallListAppModel...]; 获取列表失败, 在NSError里有记录.
 */
+ (void)PublicRequestOffersOpenData:(BOOL)PublicCallRewarded PublicCallPage:(NSInteger)PublicCallRequestPage PublicCallCount:(NSInteger)PublicCallAdCountPage PublicCallRevievedBlock:(void (^)(NSArray*, NSError *))PublicCallRecievedBlock;

/*
 *\这个默认请求第1页数据，每页10个
 */
+ (void)PublicCallRequestOffersOpenData:(BOOL)PublicCallRewarded PublicCallRevievedBlock:(void (^)(NSArray*, NSError *))PublicRecievedBlock;

/*! 安装积分墙中的APP
 * \param app 通过requestOffersOpenData获得的APP
 */
+ (void)PublicCallUserInstallApp:(PublicCallListAppModel *)PublicCallApp;

/*
 *关闭积分墙
 */
+ (void)PublicCallClose;
@end


