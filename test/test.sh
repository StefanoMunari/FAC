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
	$program $filename
done
 
