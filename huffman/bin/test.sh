#!/bin/bash

in_file=
out_file="../out.bin"
dec_file="../decoded.txt"

for t in $(ls ../tests/*.txt)
do
	in_file=$t
	./main "encode" "$in_file" "$out_file" &> /dev/null
	./main "decode" "$out_file" "$dec_file" &> /dev/null
	diff "$in_file" "$dec_file" -q &> /dev/null
	if [ $? -eq 0 ]
	then
		echo Test $in_file passed!
	else
		echo Test $in_file failed!
	fi
done
