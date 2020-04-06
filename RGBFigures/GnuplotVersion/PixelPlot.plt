set term png
set datafile separator ' '
set size ratio -1
set xrange [0:199] # there are 200 pixel in x direction
set yrange [0:99] # there are 100 pixel in y direction

set output "PInitialImage.png"
plot 'DataInitialImage.txt' w rgbimage

set output "PChangedImage_0.png"
plot 'DataChangedImage_0.txt' w rgbimage

set output "PChangedImage_1.png"
plot 'DataChangedImage_1.txt' w rgbimage

set output "PChangedImage_2.png"
plot 'DataChangedImage_2.txt' w rgbimage

set output "PChangedImage_3.png"
plot 'DataChangedImage_3.txt' w rgbimage

set output "PChangedImage_4.png"
plot 'DataChangedImage_4.txt' w rgbimage


