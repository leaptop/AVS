set terminal png size 500, 350 font 'Verdana, 10'
set title ""
set ylabel "t(sec)"
set xlabel "n"
set output 'lab3(100).png'
plot 'first.txt' using 1:2 with linespoints lw 1 lt rgb 'green' title 'clock()', 'second.txt' using 1:2 with linespoints lw 1 lt rgb 'red' title 'grttimeofday()', 'third.txt' using 1:2 with linespoints lw 1 lt rgb 'purple' title 'tsc'
