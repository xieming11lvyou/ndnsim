set terminal postscript portrait enhanced lw 2 "Helvetica" 14

set size 2,1

#-------------------------------------------------------
#set terminal png  xC9C9C7
#set output "node_100_0.99.png"
set out "Result_0.95.eps"
#set title "Percentage over Time"
set grid
set ylabel "Percentage (%)"
set yrange [0:100]
set ytics 10
set mytics 2
set xlabel "Time (s)"
set xrange [1500:1800]
set xtics 25
set mxtics 2
#set style fill transparent solid 0.4
set style data lines
set key center at 1530,90

#plot "NODE100_ONDN_10.txt" using 1:2 with l lt 1 lw 3 lc rgb "orange-red" title "ONDN - 10 Producers",\
 #"NODE100_ONDN_15.txt" using 1:2 with l lt 2 lw 3 lc rgb "orange-red" title "ONDN - 15 Producers",\
 #"NODE100_ONDN_20.txt" using 1:2 with l lt 3 lw 3 lc rgb "orange-red" title "ONDN - 20 Producers",\
 #"NODE100_IP_3_10.txt" using 1:2 with l lt 1 lw 2 lc rgb "green" title "NDN - 10 Producers",\
 #"NODE100_IP_3_15.txt" using 1:2 with l lt 2 lw 2 lc rgb "green" title "NDN - 15 Producers",\
 #"NODE100_IP_3_20.txt" using 1:2 with l lt 3 lw 2 lc rgb "green" title "NDN - 20 Producers",\
 #"NODE100_IP_random_10.txt" using 1:2 with l lt 1 lw 2 lc rgb "purple" title "IP - 10 Producers",\
 #"NODE100_IP_random_15.txt" using 1:2 with l lt 2 lw 2 lc rgb "purple" title "IP - 15 Producers",\
 #"NODE100_IP_random_20.txt" using 1:2 with l lt 3 lw 2 lc rgb "purple" title "IP - 20 Producers"
 plot "delay.txt" using 1:(($0+1)*100/1001)  with lines lt 1 title "1 Producer",\
 "delay1.txt" using 1:(($0+1)*100/920)  with lines lt 3 title  "3 Producers"
