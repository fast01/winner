<html>
	<head>
		<title> hello winner </title>
	</head>
	<body>
		<h1> 
			<# 
				if param then
					out("hello %s", param.name)
				else
					out("who are you?")
				end
			#>
		</h1>
	</body>
</html>
