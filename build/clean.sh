pid=`ps -C project | grep project | awk '{print $1}'`
if [ -n "$pid" ]; then
	echo "project pid is $pid" 
	kill -9 $pid
else
	echo "project not found"
fi
