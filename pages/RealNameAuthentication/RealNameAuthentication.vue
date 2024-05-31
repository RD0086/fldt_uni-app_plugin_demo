/**
 * 实名认证
 */
<template>
	<view>
		<form @submit="formSubmit">
			<view class="infos">
				<view class="title">姓名:</view>
				<input class="uni-input" name="mCertName" placeholder="请输入姓名" value="张三" />

				<view class="title">身份证号:</view>
				<input class="uni-input" type="idcard" name="mCertNo" placeholder="请输入身份证号" value="421126200105134227" />
			</view>
			
			<view class="checkbox-row">
			  <checkbox-group @change="checkboxChange">
			    <label class="checkbox-label" v-for="(item, index) in items" :key="item.value">
			      <checkbox :value="item.value" :checked="isChecked(item.value)" :disabled="disableCheckbox(item.value)" style="transform:scale(0.5)"/>
			      <div class="checkbox-name">{{item.name}}</div>
			    </label>
			  </checkbox-group>
			</view>
			
			<view align="center"><button class="btSubmit" form-type="submit">实人认证</button></view>
		</form>
		<div align="center">
			<textarea :value="msg"/>
		</div>
	</view>
</template>

<script>
	import CommUtil from '../../utils/CommUtil.js';
	let commUtil = new CommUtil.Commutil()
	let livingDetection = uni.requireNativePlugin('Esand-LivingDetection');
	// TODO 替换成您自己的APPCODE(切勿泄漏), 如何获取APPCODE,可参考：https://esandinfo.yuque.com/docs/share/13ad611e-b9c3-4cf8-a9a8-fe23a419312e?#
	let ALIYUN_APPCODE = 'TODO'; // 阿里云网关APPCODE

// 从一砂云接入, 可参考文档： https://esandinfo.yuque.com/yv6e1k/aa4qsg/ghtqp7
let ES_APPCODE = 'TODO'; // 一砂云网关APPCODE 
let ES_SECRET_KEY = 'TODO';// 一砂云网关密钥
let SECRET_KEY = '';
export default {
	data() {
		return {
			mCertNo: '4211262000105134227',
			mCertName:'张三',
			msg: 'logs',
			items: [
				{
					value: '1',
					name: '远近'
				},
				{
					value: '2',
					name: '眨眼'
				},
				{
					value: '3',
					name: '摇头'
				},
				{
					value: '4',
					name: '点头'
				},
				{
					value: '5',
					name: '张嘴'
				},
				{
					value: '6',
					name: '炫彩'
				}
			],
			livingType: 2
		};
	},
	methods: {
		formSubmit: function(e) {
			/**
			 * 1. 认证初始化
			 * livingDetectResult对象包括如下几个字段
			 * {
			 *		"code": ”ELD_SUCCESS“,    -- ELD_SUCCESS：成功，ELD_FAILED：失败，ELD_PARAME_ERROR：参数异常，ELD_EXCEPTION：发生异常
			 * 		"msg":”成功“, -- 执行结果描述
			 * 		"data": "......" -- 执行结果数据
			 * }
			 */
			let livingDetectResult = livingDetection.verifyInit({ livingType: this.livingType });
			console.log("初始化返回：" + JSON.stringify(livingDetectResult))
			if (livingDetectResult.code != 'ELD_SUCCESS') {
				this.msg = '活体检测初始化失败：' + livingDetectResult.msg;
				return;
			}
			
			// 判断是从一砂云接入还是阿里云接入
			let serverURL = "https://edis.esandcloud.com/gateways?APPCODE=" + ES_APPCODE + "&ACTION=livingdetection/rpverify/init";
			SECRET_KEY = ES_SECRET_KEY;
			if (ES_APPCODE == '' || ES_APPCODE == 'TODO') {
				serverURL = 'http://apprpv.market.alicloudapi.com/init';
				SECRET_KEY = ALIYUN_APPCODE;
			}
			
			let that = this;
			/**
			 * 2. 请求阿里云初始化接口获取token（为了保护APPCODE,次端逻辑应该放在服务器端）
			 * 参考文档：https://market.aliyun.com/products/57124001/cmapi00046021.html#sku=yuncode4002100001
			 */
			uni.request({
				url: serverURL,
				method: 'POST',
				header: {
					Authorization: 'APPCODE ' + SECRET_KEY,
					'X-Ca-Nonce': commUtil.randomString(8), // 防重放攻击
					'Content-Type': 'application/x-www-form-urlencoded; charset=UTF-8'
				},
				data: {
					//初始化接口返回参数
					initMsg: livingDetectResult.data,
					certNo: e.detail.value.mCertNo,
					certName: e.detail.value.mCertName
				},
				success: res => {
					if (res.data.code != '0000') {
						console.log("服务器返回：" + JSON.stringify(res))
						that.msg = '获取token失败：' + res.data.msg;
						return;
					}

					/**
					 * 3. 发起活体检测
					 */
					livingDetection.startLivingDetect({ token: res.data.token }, function(livingDetectResult) {
						/**
						 * livingDetectResult对象包括如下几个字段
						 * {
						 *		"code": ”ELD_SUCCESS“,    -- ELD_SUCCESS：成功，ELD_FAILED：失败，ELD_PARAME_ERROR：参数异常，ELD_EXCEPTION：发生异常，ELD_TIMEOUT：执行超时，ELD_PERMISSION：无法获取相机授权
						 * 		"msg":”成功“, -- 执行结果描述
						 * 		"data": "......" -- 执行结果数据
						 * 		"token": "" -- 本次认证token
						 * }
						 */
						/**
						 * 4. 请求阿里云获取服务器端活体检测结果（为了保护APPCODE,次端逻辑应该放在服务器端）
						 * 参考文档：https://market.aliyun.com/products/57124001/cmapi00046021.html#sku=yuncode4002100001
						 */
						serverURL = "https://edis.esandcloud.com/gateways?APPCODE=" + ES_APPCODE + "&ACTION=livingdetection/rpverify/verify";
						if (ES_APPCODE == '' || ES_APPCODE == 'TODO') {
							serverURL = 'http://apprpv.market.alicloudapi.com/verify';
							SECRET_KEY = ALIYUN_APPCODE;
						}
						
						uni.request({
							url: serverURL,
							method: 'POST',
							header: {
								Authorization: 'APPCODE ' + SECRET_KEY,
								'X-Ca-Nonce': commUtil.randomString(8), // 防重放攻击
								'Content-Type': 'application/x-www-form-urlencoded; charset=UTF-8'
							},
							data: {
								//初始化接口返回参数
								token: livingDetectResult.token,
								verifyMsg: livingDetectResult.data
							},
							success: res => {
								console.log("服务器返回：" + JSON.stringify(res))
								that.msg = JSON.stringify(res.data);
							}
						});
					});
				}
			});
		},
		checkboxChange: function(evt) {
		    let selectedValues = evt.detail.value;
		    if (selectedValues.length > 4) {
		      // 如果选择超过4个，则只保留前4个选择
		      selectedValues = selectedValues.slice(0, 4);
		    }
		    // 更新数据，确保只存储最多4个选择
		   this.livingType = parseInt(selectedValues.join(''));
			
		  },
		   isChecked: function(value) {
		      return this.livingType.toString().includes(value);
		    },
		    disableCheckbox: function(value) {
		      return this.livingType.toString().length >= 4 && !this.isChecked(value);
		    }
		}
};
</script>

<style>
.btSubmit {
	margin-top: 300rpx;
	margin: 100rpx 150rpx 0rpx 100rpx;
}

.infos {
	margin: 20rpx 80rpx 0rpx 100rpx;
}

.title {
	margin-bottom: 15rpx;
	margin-top: 10rpx;
}

input {
	margin-top: 20rpx;
	margin-bottom: 30rpx;
	background-color: aliceblue;
}
.content {
	position: absolute;
	width: 100%;
	height: 100%;
}

.esand_logo {
	margin-top: 100rpx;
	margin-bottom: 100rpx;
	width: 200rpx;
	height: 200rpx;
	align-self: center;
}

label {
	display: block;
	margin-top: 10rpx;
}

.checkbox-row {
   display: flex;
   flex-wrap: nowrap; /* 禁止换行 */
  
 }
 
 .checkbox-label {
   display: inline-flex;
   align-items: center;
   margin-right: 1px;
 }
 
 .checkbox-name {
   margin-left: 1px;
   font-size: 0.9rem;
 }

textarea {
	margin-top: 20rpx;
	height: 800rpx;
	font-size: small;
}
</style>
