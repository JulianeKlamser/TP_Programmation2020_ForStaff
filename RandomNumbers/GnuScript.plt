#this is a comment

set term png # Images are saved as png

set style line 1 linetype 2 linecolor rgb "red" linewidth 3 #define style of line 1
set style line 2 linecolor rgb 'orange' linetype 1 linewidth 2 pointtype 2 pointsize 0.6 #define style of line 2
set xlabel "array index" font ",15"

# First plot - just a line
set output "./Plot1.png" #output to any filename.png you want
set ylabel "Random integer number" font ",15"
plot "./RandomInt0To50.dat" with lines linestyle 1 title 'Generated from RandomInt0To50.dat'

# Second plot  - a line with plot markers
set output "./Plot2.png"
set ylabel "Random double number" font ",15"
plot "./RandomDoubleList.dat" with linespoints linestyle 2 title "Generated from RandomDoubleList.dat"

# Third plot - first and second plot together in one image
set output "./Plot3.png"
set ylabel "Random numbers" font ",15"
plot "./RandomDoubleList.dat" with linespoints linestyle 2 title "Generated from RandomDoubleList.dat", \
    "./RandomInt0To50.dat" with lines linestyle 1 title 'Generated from RandomInt0To50.dat'
