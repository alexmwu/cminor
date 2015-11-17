#!/bin/sh

#Uncomment > $testfile.out for file output
#Comment out > /dev/null to see output to stdout
for testfile in good*.cminor
do
	if ./cminor -parse $testfile > /dev/null # > $testfile.out
	then
		echo "$testfile success (as expected)"
	else
		echo "$testfile failure (INCORRECT)"
	fi
done

for testfile in bad*.cminor
do
	if ./cminor -parse $testfile > /dev/null # > $testfile.out
	then
		echo "$testfile success (INCORRECT)"
	else
		echo "$testfile failure (as expected)"
	fi
done
