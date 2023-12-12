class Commutil {
		
	constructor() {
	}
	/**
	 * 生成随机字符串
	 * @param {Object} len 字符串长度
	 */
	randomString(len) {
	　　len = len || 32;
	　　var $chars = 'ABCDEFGHJKMNPQRSTWXYZabcdefhijkmnprstwxyz2345678';
	　　var maxPos = $chars.length;
	　　var randomStr = '';
	　　for (let i = 0; i < len; i++) {
	　　　　randomStr += $chars.charAt(Math.floor(Math.random() * maxPos));
	　　}
		
		randomStr = 'LDT_' + randomStr;
	　　return randomStr;
	}
}

export default {  
    Commutil
}