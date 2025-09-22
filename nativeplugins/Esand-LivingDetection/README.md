
![DEMO](https://rd-esand-dev.oss-cn-shanghai.aliyuncs.com/imgs/fldt/demo.gif?OSSAccessKeyId=LTAI5tNZJG7Rz5icyxCpxDNg&Expires=2111946097&Signature=0NfltyR72r81CJ40c7TFj8oQOLs%3D)


![WILL](https://rd-esand-dev.oss-cn-shanghai.aliyuncs.com/imgs/fldt/will.gif?OSSAccessKeyId=LTAI5tNZJG7Rz5icyxCpxDNg&Expires=1794525118&Signature=%2Bg2Wc%2FqDBzrxi%2B7l3r5%2BfKQqnRo%3D)
## 插件API接口说明

#### android 调用初始化
```java
/**
 * SDK初始化。为了提升性能，建议在在application提前调用，否则获取token时间会比较长
 */
LivingDetectionModule.Init();
```

#### 认证初始化
```java
/**
 * 认证初始化
 * @param options(JSONObject), 包括如下字段：
 *     livingType：认证类型  1：远近，2：眨眼，3：摇头，4: 点头，5:张嘴，6: 炫彩
 *                  支持多动作，如传入12表示先做远近活体，后做眨眼活体，一次最多支持4组动作
 *     keyType: string, 密钥类型 (留空即可)
 *     isAutoUploadVerifyMsg: boolean, 是否自动上传verifyMsg (留空即可)
 *     navigate: boolean, 是否显示导航条
 *     textColor：界面样式-字体颜色
 *     takeMultiImg: boolean , true : 获取结果时候返回多张照片，默认为false
 *     isRecordVideo: boolean , 是否录制视频，默认为false，如果设置为true, 那么将会在认证结束的时候将会返回视频存储的本地绝对路径
 *     progressColor：界面样式-进度条颜色
 *     progressBgColor：界面样式-进度条背景颜色
 *     progressStaGradient：界面样式-进度条渐变开始颜色
 *     progressEndGradient：界面样式-进度条渐变结束颜色
 *     backGroundColor：界面样式-页面背景颜色
 *     circleBackWidth：界面样式-进度条宽度
 *     language: 界面显示的语言（不分大小写），“CN”:中文，“TCN”： 繁体中文， “JP”: 日文，"KR": 韩文， “EN”: 英文
 *     visitedStepBorderDotColor：多动作导航的边框颜色, 如 #FFC0CB
 *     visitedStepFillDotColor： 完成步骤点的填充颜色, 如 #FFC0CB
 *     nextStepBorderDotColor： 下一步骤点的填充颜色, 如 #FFC0CB
 *     nextStepFillDotColor： 下一步骤点的填充颜色, 如 #FFC0CB
 *     visitedStepSeparatorColor：  当前步骤条的颜色, 如 #FFC0CB
 *     nextStepSeparatorColor： 下一步步骤条的颜色, 如 #FFC0CB
 *     exitIcon： 退出按钮图片（BASE64字符串）
 * @return livingDetectResult 对象包括如下几个字段
 * {
 *      "code": ”ELD_SUCCESS“, -- ELD_SUCCESS：成功，ELD_FAILED：失败，ELD_PARAME_ERROR：参数异常，ELD_EXCEPTION：发生异常，ELD_UNSUPPORT：不支持此活体类型
 *      "msg":”成功“, -- 执行结果描述
 *      "data": "......" -- 执行结果数据
 * }
 */
 livingDetectResult LivingDetectionModule.verifyInit(options);
 ```

 #### 发起活体认证
 ```java
/*
 * 执行活体认证
 * @param options(JSONObject), 包括如下字段：
 *     token：认证初始化服务器端返回的数据
 * @param callback 执行结果回调，传入参数为对象 livingDetectResult 对象包括如下几个字段
 * {
 *    "code": ”ELD_SUCCESS“, -- ELD_SUCCESS：成功，ELD_FAILED：失败，ELD_PARAME_ERROR：参数异常，ELD_EXCEPTION：发生异常，ELD_TIMEOUT：执行超时，ELD_PERMISSION：无法获取相机授权，ELD_CANCEL: 用户主动退出
 *     "msg":”成功“, -- 执行结果描述
 *    "data": "......" -- 执行结果数据
 *    "token": "" -- 本次认证token
 *    "videoPath": "/storage/xxxxx" -- 录制视频存储的绝对路径
 * }
 */
LivingDetectionModule.startLivingDetect(options, callback);
```

## 页面样式修改
#### Android
![ANDRIOID STYLE](https://rd-esand-dev.oss-cn-shanghai.aliyuncs.com/imgs/fldt/ldt_style.png?OSSAccessKeyId=LTAI5tNZJG7Rz5icyxCpxDNg&Expires=1794525279&Signature=cnQPMndXSsNSysuL0Ef%2F3HC1fJA%3D)

#### IOS
![IOS STYLE](https://rd-esand-dev.oss-cn-shanghai.aliyuncs.com/imgs/fldt/ldt_style_ios.png?OSSAccessKeyId=LTAI5tNZJG7Rz5icyxCpxDNg&Expires=1794525308&Signature=qb1xTM7u2JaVcJZHUx3Hb4PIJ2k%3D)



### 联系我
---
有任何问题或疑问可联系我

瑞德

`Wechat Number:` esand_info </br>
`QQ Number:` 3626921591 </br>
`Phone Number:` +8613691664797</br>
`Email:` reid.li@foxmail.com</br>
`Telegram:` [@R90000000](https://t.me/R90000000)</br>


Wechat QR CODE :

![QRCODE](https://rd-esand-dev.oss-cn-shanghai.aliyuncs.com/imgs/comms/qrcode.jpeg?OSSAccessKeyId=LTAI5tNZJG7Rz5icyxCpxDNg&Expires=2111946039&Signature=Bry0xT9G22aThkQZzGzNqfLdxq0%3D)
