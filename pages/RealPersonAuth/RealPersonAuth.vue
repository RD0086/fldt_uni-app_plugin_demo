/**
 * 实人认证
 */
<template>
  <view class="container">
    <view class="form-row">
      <view class="form-item">
        <label class="form-label">姓名</label>
        <input 
          type="text" 
          v-model="userInfo.name" 
          placeholder="请输入真实姓名"
          class="form-input"
          :class="{ 'input-error': nameError }"
        />
        <view v-if="nameError" class="error-message">请输入有效的姓名</view>
      </view>

      <view class="form-item">
        <label class="form-label">身份证号</label>
        <input 
          type="text" 
          v-model="userInfo.idNumber" 
          placeholder="请输入18位身份证号"
          class="form-input"
          :class="{ 'input-error': idNumberError }"
          maxlength="18"
          @input="validateIdNumber"
        />
        <view v-if="idNumberError" class="error-message">身份证格式不正确</view>
      </view>
    </view>

    <view class="checkbox-row">
      <checkbox-group @change="checkboxChange">
        <label class="checkbox-label" v-for="(item, index) in items" :key="item.value">
          <checkbox 
            :value="item.value" 
            :checked="isChecked(item.value)" 
            :disabled="disableCheckbox(item.value)" 
            style="transform:scale(0.5)"
          />
          <div class="checkbox-name">{{item.name}}</div>
        </label>
      </checkbox-group>
    </view>

    <!-- 认证按钮 -->
    <view class="btn-row">
      <button 
        type="primary" 
        class="primary item" 
        @tap="startVerify"
        :disabled="!formValid"
      >
        发起实人认证
      </button>
    </view>

    <!-- 结果展示 -->
    <view class="result-area">
      <textarea 
        v-model="msg" 
        placeholder="认证结果将显示在这里" 
        class="result-textarea"
        readonly
      />
    </view>
  </view>
</template>


<script>
	import CommUtil from '../../utils/CommUtil.js';
	let commUtil = new CommUtil.Commutil()

  let livingDetection = uni.requireNativePlugin("Esand-LivingDetection")
 // TODO 替换成您自己的APPCODE(切勿泄漏), 如何获取APPCODE,可参考：https://esandinfo.yuque.com/docs/share/13ad611e-b9c3-4cf8-a9a8-fe23a419312e?#
 let ALIYUN_APPCODE = 'TODO'; // 阿里云网关APPCODE

 // 从一砂云接入, 可参考文档： https://esandinfo.yuque.com/yv6e1k/aa4qsg/ghtqp7
 let ES_APPCODE = 'TODO'; // 一砂云网关APPCODE
 let ES_SECRET_KEY = 'TODO';// 一砂云网关密钥

 let SECRET_KEY = '';
  export default {
    data() {
      return {
		userInfo: {
		  name: '',
		  idNumber: ''
		},
		nameError: false,
		idNumberError: false,
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
		  }
        ],
        livingType: '2'
      }
    },computed: {
    formValid() {
      return this.userInfo.name.trim() && 
             !this.nameError && 
             !this.idNumberError &&
             this.userInfo.idNumber.length === 18;
    }
  },
    methods: {
      startVerify: function(e) {
				console.log("发起活体检测")
        let livingDetection = uni.requireNativePlugin("Esand-LivingDetection")
				if (livingDetection == undefined) {
					console.error("请检查是否运行为自定义基座，并且把活体检测打包进基座了")
				}
		// let a = uni.env.USER_DATA_PATH ;
        /**
         * 1. 认证初始化
         * @param options(JSONObject), 包括如下字段：
         *     livingType：认证类型  1：远近，2：眨眼，3：摇头，4: 点头，5:张嘴，6：炫彩
         * @return livingDetectResult 对象包括如下几个字段
         * {
         *    "code": ”ELD_SUCCESS“, -- ELD_SUCCESS：成功，ELD_FAILED：失败，ELD_PARAME_ERROR：参数异常，ELD_EXCEPTION：发生异常，ELD_UNSUPPORT：不支持此活体类型
         *    "msg":”成功“, -- 执行结果描述
         *    "data": "......" -- 执行结果数据
         * }
         */
        let livingDetectResult = livingDetection.verifyInit({"livingType":parseInt(this.livingType)});
				console.log("初始化返回：" + JSON.stringify(livingDetectResult))
        if (livingDetectResult.code != 'ELD_SUCCESS') {
          this.msg = '活体检测初始化失败：' + livingDetectResult.msg
          return;
        }

		// 判断是从一砂云接入还是阿里云接入
		let serverURL = "https://edis.esandcloud.com/gateways?APPCODE=" + ES_APPCODE + "&ACTION=livingdetection/livingdetect/init";
		SECRET_KEY = ES_SECRET_KEY;
		if (ES_APPCODE == '' || ES_APPCODE == 'TODO') {
			serverURL = 'https://apprpv.market.alicloudapi.com/init';
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
            'Authorization': 'APPCODE ' + SECRET_KEY,
            'X-Ca-Nonce': commUtil.randomString(8),  // 防重放攻击
            'Content-Type': 'application/x-www-form-urlencoded; charset=UTF-8'
          },
          data: {
            //初始化接口返回参数
			'certName':this.userInfo.name,
			'certNo':this.userInfo.idNumber,
            'initMsg': livingDetectResult.data,
          },
          success: (res) => {
						console.log("服务器返回：" + JSON.stringify(res))
            if (res.data.code != '0000') {
              that.msg = '获取token失败：'+res.data.msg
              return;
            }

            /**
             * 3. 发起活体检测
             */
            livingDetection.startLivingDetect({"token":res.data.token},function(livingDetectResult) {
              /**
               * livingDetectResult 对象包括如下几个字段
               * {
               *    "code": ”ELD_SUCCESS“, -- ELD_SUCCESS：成功，ELD_FAILED：失败，ELD_PARAME_ERROR：参数异常，ELD_EXCEPTION：发生异常，ELD_TIMEOUT：执行超时，ELD_PERMISSION：无法获取相机授权，ELD_CANCEL: 用户主动退出
               *    "msg":”成功“, -- 执行结果描述
               *    "data": "......" -- 执行结果数据
               *    "token": "" -- 本次认证token
               * }
               */

              /**
               * 4. 请求阿里云获取服务器端活体检测结果（为了保护APPCODE,次端逻辑应该放在服务器端）
               * 参考文档：https://market.aliyun.com/products/57124001/cmapi00046021.html#sku=yuncode4002100001
               */
              serverURL = "https://edis.esandcloud.com/gateways?APPCODE=" + ES_APPCODE + "&ACTION=livingdetection/livingdetect/verify";
              if (ES_APPCODE == '' || ES_APPCODE == 'TODO') {
              	serverURL = 'https://apprpv.market.alicloudapi.com/verify';
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
                  //初始化接口返回参数
                  "token": livingDetectResult.token,
                  'verifyMsg': livingDetectResult.data,
                },
                success: (res) => {
                  that.msg = JSON.stringify(res.data);
                },
              });
            });
          },
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
	    },
		validateIdNumber() {
		      // 身份证号验证正则（支持18位身份证）
		      const idPattern = /^[1-9]\d{5}(18|19|20)\d{2}(0[1-9]|1[0-2])(0[1-9]|[12][0-9]|3[01])\d{3}[0-9Xx]$/;
		      this.idNumberError = !idPattern.test(this.userInfo.idNumber);
		    }
    }
  }
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
   margin-top: 20rpx;

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
 .btn-row{
	 margin-top: 40rpx;
 }


  textarea {
    margin-top: 20rpx;
    height: 800rpx;
    font-size: small;
  }
</style>
