#this is a comment

set term png # Images are saved as png

set style line 1 linetype 2 linecolor rgb "red" linewidth 3 #define style of line 1
set style line 2 linecolor rgb 'orange' linetype 1 linewidth 2 pointtype 2 pointsize 0.6 #define style of line 2
set style line 3 linecolor rgb 'green' linetype 1 linewidth 2 pointtype 1 pointsize 0.6 #define style of line 2
set xlabel 'v = sqrt(v_x^2+v_y^2)' font ",15"
set ylabel "P(v)" font ",15"


set output "./Plot.png"
plot "./MaxwellBoltzmannHistogram1.dat" with linespoints linestyle 2 title "Histogram 1", \
    "./MaxwellBoltzmannHistogram2.dat" with linespoints linestyle 3 title "Histogram 2", \
    "./MaxwellBoltzmannTheory.dat" with lines linestyle 1 title 'Maxwell-Boltzmann distribution'
