set terminal postscript portrait enhanced lw 2 "Helvetica" 14

set size 2,1

#-------------------------------------------------------
#set terminal png  xC9C9C7
#set output "node_100_0.99.png"
set out "wifi-default_0.99.eps"
#set title "Percentage over Time"
set ylabel "Percentage (%)"
set yrange [0:100]
set xlabel "Time (s)"
set xrange [1500:2500]
set style data lines

plot "node_100_n_4_0.99_3.txt" using 1:2 with lines lt 1 title "2 Producer",\
 "node_100_n_4_0.99_4.txt" using 1:2 with lines lt 3 title  "3 Producers",\
 "node_100_n_4_0.99_5.txt" using 1:2 with lines lt 4 title "4 Producers",\
 "node_100_n_4_0.99_6.txt" using 1:2 with lines lt 5 title "5 Producers",\
 "node_100_n_4_0.99_7.txt" using 1:2 with lines lt 6 title "6 Producers",\
 "node_100_n_4_0.99_8.txt" using 1:2 with lines lt 7 title "7 Producers",\
 "node_100_n_4_0.99_9.txt" using 1:2 with lines lt 8 title "8 Producers"
 #"node_100_n_4_0.99_41.txt" using 1:2 with lines lt 9 title "40 Producers"