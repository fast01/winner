#!/bin/bash
root_dir=`pwd`
protocoltypes=`ls .`

echo $protocoltypes
for dir in $protocoltypes; do
	if [ $dir != 'script' -a -d $dir ]; then
		cd $dir
		echo "### Enter $dir ###"
		if [ -e build.sh ]; then
			./build.sh
		fi
		echo "### Leave $dir ###"
		cd $root_dir
	fi
done

