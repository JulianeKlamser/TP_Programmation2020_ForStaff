set term png
set datafile separator ' '


set output "RandomWalkSample.png"
plot 'RandomWalk.dat' w l lw 2

set output "POfx.png"
plot 'PofX.dat' w l lw 4, 'PofXTheory.dat' w l lw 4


set size ratio -1
set xrange [0:] # there are 200 pixel in x direction
set yrange [0:] # there are 100 pixel in y direction

set output "PofXandY.png"
plot 'Histogram.dat' w rgbimage

