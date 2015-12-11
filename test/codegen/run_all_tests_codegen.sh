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
  filename="${genfile%.*}"
  if ! gcc $genfile ../../library.o -o "$filename.out"
  then
    echo "$testfile failure (COMPILING/LINKING)"
  fi
done

#for outfile in good*.out
#do
  ##eval "$filename"
#done

