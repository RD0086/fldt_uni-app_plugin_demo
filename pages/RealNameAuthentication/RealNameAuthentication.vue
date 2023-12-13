/**
 * 实名认证
 */
<template>
	<view>
		<form @submit="formSubmit">
			<view class="infos">
				<view class="title">姓名:</view>
				<input class="uni-input" name="mCertName" placeholder="请输入姓名" value="" />

				<view class="title">身份证号:</view>
				<input class="uni-input" type="idcard" name="mCertNo" placeholder="请输入身份证号" value="" />
			</view>
			<view class="uni-list">
				<radio-group @change="radioChange">
					<label class="uni-list-cell uni-list-cell-pd radio-group" v-for="(item, index) in items" :key="item.value">
						<radio :value="item.value" :checked="index === livingType - 1" style="transform:scale(0.5)" />
						<view style="display: inline-block;">{{ item.name }}</view>
					</label>
				</radio-group>
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
// TODO 替换成您的APPCODE
let livingDetection = uni.requireNativePlugin('Esand-LivingDetection');
// TODO 替换成您自己的APPCODE(切勿泄漏), 如何获取APPCODE,可参考：https://esandinfo.yuque.com/docs/share/13ad611e-b9c3-4cf8-a9a8-fe23a419312e?#
let APPCODE = 'TODO';
export default {
	data() {
		return {
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
			if (livingDetectResult.code != 'ELD_SUCCESS') {
				this.msg = '活体检测初始化失败：' + livingDetectResult.msg;
				return;
			}
			let that = this;
			/**
			 * 2. 请求阿里云初始化接口获取token（为了保护APPCODE,次端逻辑应该放在服务器端）
			 * 参考文档：https://market.aliyun.com/products/57124001/cmapi00046021.html#sku=yuncode4002100001
			 */
			uni.request({
				url: 'http://apprpv.market.alicloudapi.com/init',
				method: 'POST',
				header: {
					Authorization: 'APPCODE ' + APPCODE,
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
						uni.request({
							url: 'http://apprpv.market.alicloudapi.com/verify',
							method: 'POST',
							header: {
								Authorization: 'APPCODE ' + APPCODE,
								'X-Ca-Nonce': commUtil.randomString(8), // 防重放攻击
								'Content-Type': 'application/x-www-form-urlencoded; charset=UTF-8'
							},
							data: {
								//初始化接口返回参数
								token: livingDetectResult.token,
								verifyMsg: livingDetectResult.data
							},
							success: res => {
								that.msg = JSON.stringify(res.data);
							}
						});
					});
				}
			});
		},
		radioChange: function(evt) {
			for (let i = 0; i < this.items.length; i++) {
				if (this.items[i].value === evt.target.value) {
					this.livingType = i + 1;
					break;
				}
			}
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
	margin: 100rpx 80rpx 0rpx 100rpx;
}

.title {
	margin-bottom: 15rpx;
	margin-top: 10rpx;
}

input {
	margin-top: 20rpx;
	margin-bottom: 30rpx;
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

.radio-group {
	display: inline-block;
}

textarea {
	margin-top: 20rpx;
	height: 800rpx;
	font-size: small;
}
</style>
