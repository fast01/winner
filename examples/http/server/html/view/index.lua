<html>
	<head>
		<title> hello winner </title>
	</head>
	<body>
		<h1> 
			<# 
				if param and param.name then
					out("hello %s</br>", param.name)
					out("you age is %d", param.age or 0)
				else
					out("who are you?")
				end
			#>
		</h1>
	</body>
</html>
