#!/bin/bash

echo "Delay in centiseconds (100cs = 1s)"
read delay

echo "Resizing images"
for i in $(ls *.png); do
	convert $i -gravity center -background white -extent 1500x900 extended$i
done
echo "Resizing done!"
echo "Creating gif"
convert -delay $delay -loop 0 extended*.png animation.gif
echo "Remove dot and old pngs?"
read ans

case $ans in
	'y' | 'Y' | 'Yes' | 'yes')
		rm *.dot *.png
		;;
esac
