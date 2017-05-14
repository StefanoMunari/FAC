#!/bin/bash
program=../src/bin/fac

if [ ! -f "$program" ]
then
	echo "File $program does not exists. I will create it\n"
	cd ../src/
	make
	cd ../test/
fi
echo "Executing all examples in f directory.."
for filename in ../examples/ex*.f; do
	echo "Executing $filename.."
	output=`$program $filename 2>&1 >/dev/null`
	res=$?
	if [ $res -ne 0 ] 
	then
		echo "#################################"
		echo "$filename test was not successful with code $res ..."
		echo $output
		echo "#################################"
	fi
done
 
