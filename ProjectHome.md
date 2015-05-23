A server side ecmascript templating platform for serving websites.  In short, PHP without php.

## A simple example ##
For example, the following script
```
<~
	var title = 'hello, world!';
	var names = ['joe','nancy','bill'];
~>

<html>
	<head>
		<title><=title=></title>
	</head>
	<body>
		<h3>Say hello everyone!</h3>
		<: for (var i in names) :>
			<p><=names[i]=> says Hi!</p>
		<: endfor :>
	</body>
</html>
```
would generate the following:
```
<html>
	<head>
		<title>hello, world!</title>
	</head>
	<body>
		<h3>Say hello everyone!</h3>
			<p>joe says Hi!</p>
			<p>nancy says Hi!</p>
			<p>bill says Hi!</p>
	</body>
</html>
```