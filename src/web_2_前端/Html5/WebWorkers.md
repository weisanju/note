# WebWorkers出现线程

## webworker简介

1. 创建后台运行线程API

2. webwork对象创建

   `var worker = new Worker("worker.js")`

3. 后台线程不能访问 窗口和页面对象,所有不能使用 document,window对象

4. 接收发送消息

   `worker.onmessage=function(event){....}`

   `worker.postMessage(message);`

5. 线程中可以在创建线程, 嵌套

6. 线程中可用的变量函数类

   1. self:本线程范围的作用域
   2. postMessage(message):向线程创建者发送消息
   3. onmessage:接收消息的句柄
   4. importScripts(urls):导入其他JavaScript脚本文件,可以导入多个
   5. navigator对象:具有appName,platform,userAgent,appVersion属性
   6. sessionStorage/localStorage
   7. XMLHttpRequest
   8. setTimeout()/setInterval()
   9. close:结束本线程
   10. eval(),isNaN(),escape()
   11. object
   12. websockets

6. 示例

```html
<!DOCTYPE html>
<html>
	<head>
		<meta charset="UTF-8">
		<title></title>
		<script type="text/javascript">
			var worker = new Worker("sumcalculator.js");
			worker.onmessage = function(event){
				alert("合计为:"+event.data)
			}
			function calculate(){
				var num = parseInt(document.getElementById('num').value,10);
				worker.postMessage(num);
			}
		</script>
	</head>
	<body>
		<h1>从1到给定数值的和</h1>
		<input type="number"  id="num" >
		<input type="button" name="" id="" value="计算" onclick="calculate()" />
	</body>
</html>

/*sumcalculator.js*/
onmessage = function(event){
	var num = event.data;
	var result = 0;
	for (var i=0;i<num;i++) {
		result+=i;
	}
	postMessage(result);
}

```

