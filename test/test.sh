#!/bin/bash
program=../src/bin/fac

if [ ! -f "$program" ]
then
	echo "File $program does not exists. I will create it\n"
	cd ../src/
	make
	cd ../test/
fi
print "Executing all examples in f directory.."
for filename in ../examples/ex*.f; do
	output=`$program $filename 2>&1 >/dev/null`
	if [ $? -ne 0 ] 
	then
		echo "$filename test was not successful"
		echo $output
	fi
done
 
