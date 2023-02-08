set terminal pdf
set output "throughput.pdf"
set title "plot againts throughput and time"
set ylabel "throughput(Mbps)"
set xlabel "time "
set xrange [0:20]
set xtics 1
plot "flow1.txt" using 2:3 with linespoints, "flow2.txt" using 2:3 with linespoints, "flow5.txt" using 2:3 with linespoints, "flow7.txt" using 2:3 with linespoints

