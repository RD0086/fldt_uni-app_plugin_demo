//
//  EsLivingConfig.h
//  EsLivingDetection
//
//  Created by zhangzelong on 2022/10/24.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef enum CRYPT_KEY_TYPE{
    // 默认
    DEFAULT,
    // 上海CA
    SH_CA,
    // TEST
    TEST
}CRYPT_KEY_TYPE;

/**
 * 活体检测配置
 */
@interface EsLivingConfig : NSObject

/**
 * 设置是否自动上传verifyMsg
 * @param value true: 自动上传 false: 不上传 默认为: true
 */
-(void) SetIsAutoUploadVerifyMsg:(BOOL) value;

/**
 * 设置是否录制活体视频
 * @param value true: 录制 false: 不录制  默认为: false
 */
-(void) SetIsRecordLdtVideo:(BOOL) value;

/**
 * 设置是否在下一次开始认证时，删除认证视频
 * @param value true: 删除 false: 保留 默认为: true
 */
-(void) SetIsDeleteVideoInNext:(BOOL) value;

/**
 * 设置是否保存关键帧
 * @param value true: 保存 false: 不保存
 */
-(void) SetIsSaveImgEveryType:(BOOL) value;

/**
 * 获取是否自动上传verifyMsg
 */
-(BOOL) GetIsAutoUploadVerifyMsg;

/**
 * 获取当前是否开启活体录制
 */
-(BOOL) GetIsRecordLdtVideo;

/**
 * 获取是否在下一次开始认证时，删除认证视频
 */
-(BOOL) GetIsDeleteVideoInNext;

/**
 * 是否保存每一个活体动作的关键帧
 */
-(BOOL) GetIsSaveImgEveryType;

/**
 * 获取当前的加密密钥类型
 *  @return 当前密钥 DEFAULT, SH_CA
 */
-(CRYPT_KEY_TYPE) GetEsCryptKeyType;


-(void) SetEsCryptKeyType:(CRYPT_KEY_TYPE) keyType;

-(BOOL) GetUseStrictMode;

-(void) SetUseStrictMode:(BOOL)value;
@end

NS_ASSUME_NONNULL_END