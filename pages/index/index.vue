<template>
	<div>
		<button @click="goToLiveness">活体检测</button>
		<button @click="goToIdentityVerification">实名认证</button>
		<button @click="goToFaceRecognition">刷脸认证</button> 
	</div> 
</template>
 
 <script>
	 export default { 
		 methods: { 
			 goToLiveness() {
				 //跳转活体检测页面
				uni.navigateTo({
				url: '/pages/FacialLivenessDetection/FacialLivenessDetection'
				})
			},
			 goToIdentityVerification() {
			    // 跳转到实名认证页面
			    uni.navigateTo({
			    url: '/pages/RealNameAuthentication/RealNameAuthentication'
			    }) 
			},
			 goToFaceRecognition() {
				 // 跳转到刷脸认证页面
				uni.navigateTo({
				url: '/pages/BrushFaceAuthentication/BrushFaceAuthentication'
				})
		    },
		}	
	}
 </script>
 
 <style>
	button { 
		margin: 10px;
		padding: 10px 20px;
		background-color: #007bff;
		color: white;
		border: none; 
		border-radius: 5px; 
		cursor: pointer; 
	} 
	button:hover { 
		background-color: #0056b3; 
	}
 </style>