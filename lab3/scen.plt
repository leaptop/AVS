set terminal png size 500, 350 font 'Verdana, 10'
set title ""
set ylabel "t(sec)"
set xlabel "n"
set output 'lab3(50).png'
plot 'wtime.txt' using 1:2 with linespoints lw 1 lt rgb 'green' title 'gettimeofday', 'clock_time.txt' using 1:2 with linespoints lw 1 lt rgb 'red' title 'clock', 'tsc_time.txt' using 1:2 with linespoints lw 1 lt rgb 'purple' title 'tsc'