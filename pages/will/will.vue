/**
 * 意愿核验, 接入可参考文档：https://esandinfo.yuque.com/yv6e1k/aa4qsg/sxm686gzc3kzccyi
 */
<template>
  <view>
    <view class="btn-row">
      <button type="primary" class="primary item" @tap="startVerify()">发起意愿核验</button>
    </view>

    <div align="center">
      <textarea :value="msg"/>
    </div>
  </view>
</template>

<script>
	import CommUtil from '../../utils/CommUtil.js';
	let commUtil = new CommUtil.Commutil()

  let fldtPlugin = uni.requireNativePlugin("Esand-LivingDetection")
 // TODO 替换成您自己的APPCODE(切勿泄漏), 如何获取APPCODE,可参考：https://esandinfo.yuque.com/docs/share/13ad611e-b9c3-4cf8-a9a8-fe23a419312e?#
 let ALIYUN_APPCODE = 'TODO'; // 阿里云网关APPCODE

 // 从一砂云接入, 可参考文档： https://esandinfo.yuque.com/yv6e1k/aa4qsg/ghtqp7
 let ES_APPCODE = 'TODO'; // 一砂云网关APPCODE
 let ES_SECRET_KEY = 'TODO';// 一砂云网关密钥
 let SECRET_KEY = '';
  export default {
    data() {
      return {
        msg: 'logs'
      }
    },
    methods: {
      startVerify: function(e) {
				console.log("发起意愿核验")
        let livingDetection = uni.requireNativePlugin("Esand-LivingDetection")
				if (livingDetection == undefined) {
					console.error("请检查是否运行为自定义基座，并且把活体检测打包进基座了")
				}

				// 判断是从一砂云接入还是阿里云接入 
				let serverURL = "https://edis.esandcloud.com/gateways?APPCODE=" + ES_APPCODE + "&ACTION=livingdetection/will/getVerifyUrl";
				SECRET_KEY = ES_SECRET_KEY;
				if (ES_APPCODE == '' || ES_APPCODE == 'TODO') {
					serverURL = 'https://intverify.market.alicloudapi.com/will/getVerifyUrl';
					SECRET_KEY = ALIYUN_APPCODE;
				}
		
        let that = this;
        /**
         * 1. 获取认证链接（注意：为了保护密钥，这段代码需要写在业务服务器，然后经过业务服务器进行转发）
         * 参考文档：https://esandinfo.yuque.com/yv6e1k/aa4qsg/wxn1zpfxyahd39s5
         */
        uni.request({
          url: serverURL,
          method: 'POST',
          header: {
            'Authorization': 'APPCODE ' + SECRET_KEY,
            'X-Ca-Nonce': commUtil.randomString(8),  // 防重放攻击
            'Content-Type': 'application/x-www-form-urlencoded; charset=UTF-8'
          },
          data: {
            //初始化接口返回参数 （如果没有自己的业务ID, 填入随机字符串即可）
            'bizId': commUtil.randomString(16),
						'idName': '翠花', // 姓名 (填入正确的身份证姓名，实名核验完成后才到意愿核验)
						'idNumber': '1025591988666254299', // 身份证号
						/**
						 * 人脸活体检测组合，如：13 为先远近检测，后摇头检测 : 
									1：远近，
									2: 眨眼
									3：摇头，
									4: 点头，
									6：炫彩
						 */
						'fldtType': '12',
						'willStandText': '["本人同意办理号码开户业务","本人知情以下号码开户，知晓出售、出租、转让等方式提供电话卡供他人使用的法律风险。"]',
						'answerThreshold': 85,
						'willStandAnswer': '["{willStandText}","同意|是的|好的|没问题"]'
          },
          success: (res) => {
						console.log("服务器返回：" + JSON.stringify(res))
            if (res.data.code != '0000') {
              that.msg = '获取token失败：'+res.data.msg
              return;
            }

            /**
             * 2. 发起意愿核验
             */
            fldtPlugin.startWillVerify({"url":res.data.verifyUrl},function(result) {
              /**
               * result 对象包括如下几个字段
							 * {
							 *      "code": ”ELD_SUCCESS“, -- ELD_SUCCESS：成功，ELD_FAILED：失败，ELD_EXCEPTION：发生异常，ELD_TIMEOUT：执行超时，ELD_PERMISSION：无法获取相机授权，ELD_CANCEL: 用户主动退出, ELD_EXIT: 程序退出/页面被置于后台 （页面不可见）
							 *      "msg":”成功“, -- 执行结果描述
							 *      "data": "......" -- 认证业务ID (用于查询结果用)
							 * }
               */

              /**
               * 4. 获取核验结果（注意：为了保护密钥，这段代码需要写在业务服务器，然后经过业务服务器进行转发）
               * 参考文档：https://esandinfo.yuque.com/yv6e1k/aa4qsg/eqh6crlqogrchxfe
               */
              serverURL = "https://edis.esandcloud.com/gateways?APPCODE=" + ES_APPCODE + "&ACTION=livingdetection/will/queryVerifyResult";
              if (ES_APPCODE == '' || ES_APPCODE == 'TODO') {
              	serverURL = 'https://intverify.market.alicloudapi.com/will/queryVerifyResult';
              	SECRET_KEY = ALIYUN_APPCODE;
              }
              uni.request({
                url: serverURL,
                method: 'POST',
                header: {
                  'Authorization': 'APPCODE ' + SECRET_KEY,
                  'X-Ca-Nonce': commUtil.randomString(8),  // 防重放攻击
                  'Content-Type': 'application/x-www-form-urlencoded; charset=UTF-8'
                },
                data: {
                  "businessId": result.data
                },
                success: (res) => {
                  that.msg = JSON.stringify(res.data);
                },
              });
            });
          },
        });
      }
</script>
<style>
  .content {
    position: absolute;
    width: 100%;
    height: 100%;
  }


  label {
    display: block;
    margin-top: 10rpx;
  }

 .btn-row{
	 margin-top: 40rpx;
 }


  textarea {
    margin-top: 20rpx;
    height: 800rpx;
    font-size: small;
  }
</style>
