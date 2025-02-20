/**
 * 刷脸认证
 */
<template>
	<view >
		<view class="container">
			 <image :src="photoUrl" style="width: 150px; height: 200px;"></image> 
		</view>
	  <view class="checkbox-row">
	    <checkbox-group @change="checkboxChange">
	      <label class="checkbox-label" v-for="(item, index) in items" :key="item.value">
	        <checkbox :value="item.value" :checked="isChecked(item.value)" :disabled="disableCheckbox(item.value)" style="transform:scale(0.5)"/>
	        <div class="checkbox-name">{{item.name}}</div>
	      </label>
	    </checkbox-group>
	  </view>
	  
	  <view class="btn-row">
		<button type="primary" class="primary item" style="margin: 10px" @tap="chooseImage()">选择比对源</button>
	    <button type="primary" class="primary item" style="margin: 10px" @tap="startVerify()">开始刷脸认证</button>
		
	  </view>
	  
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
	let base64Image = ''
	export default {
	  data() {
	    return {
		  photoUrl: 'data:image/jpeg;base64,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',
	      msg: 'logs',
	      items: [{
	          value: '1',
	          name: '远近'
	        },{
	          value: '2',
	          name: '眨眼'
	        },{
	          value: '3',
	          name: '摇头'
	        },{
	          value: '4',
	          name: '点头'
	        },{
	          value: '5',
	          name: '张嘴'
	        },
			{
				value: '6',
				name: '炫彩'
			},{
		  	value: '8',
		  	name: '静默活体'
			}
	      ],
	      livingType: 2 ,
	    };
	  },
	 methods: {
	 	startVerify: function(e) {
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
	 		let serverURL = "https://edis.esandcloud.com/gateways?APPCODE=" + ES_APPCODE + "&ACTION=livingdetection/faceContrast/init";
	 		SECRET_KEY = ES_SECRET_KEY;
	 		if (ES_APPCODE == '' || ES_APPCODE == 'TODO') {
	 			serverURL = 'https://efaceid.market.alicloudapi.com/init';
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
	 					serverURL = "https://edis.esandcloud.com/gateways?APPCODE=" + ES_APPCODE + "&ACTION=livingdetection/faceContrast/verify";
	 					if (ES_APPCODE == '' || ES_APPCODE == 'TODO') {
	 						serverURL = 'https://efaceid.market.alicloudapi.com/verify';
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
	 							verifyMsg: livingDetectResult.data,
								image: base64Image,
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
		chooseImage() {
		    uni.chooseImage({
		        count: 1,
		        sizeType: ['compressed'],
		        sourceType: ['album'],
		        success: (res) => {
		            let filePath = res.tempFilePaths[0];
		            this.photoUrl = filePath;
				   console.log('图片路径',this.photoUrl)
                    plus.io.resolveLocalFileSystemURL(filePath, (entry) => {
                         entry.file((file) => {
                            let fileReader = new plus.io.FileReader();
                            fileReader.onloadend = (e) => {
								base64Image = e.target.result
                                //console.log('图片数据',base64Image) 
                            };
                           fileReader.readAsDataURL(file);
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
	  margin-top: 10rpx;
	 
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
	.container {
	    display: flex;
	    flex-direction: column;
	    justify-content: center;
	    align-items: center;
	    height: 20vh; 
	}
	.centered-image {
	    width: 200px;
	    height: 200px;
	}
</style>