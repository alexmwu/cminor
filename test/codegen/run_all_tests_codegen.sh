#!/bin/sh

#Uncomment > $testfile.out for file output
#Comment out > /dev/null to see output to stdout
for testfile in good*.cminor
do
	if ! ./cminor -codegen $testfile
	then
		echo "$testfile failure (CODEGEN)"
	fi
done

for genfile in good*.s
do
  if ! gcc $genfile
  then
    echo "$testfile failure (COMPILING/LINKING)"
  fi
done

