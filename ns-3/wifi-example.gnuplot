set terminal postscript portrait enhanced lw 2 "Helvetica" 14

set size 2,1

#-------------------------------------------------------
set terminal png  xC9C9C9
set output "sarimage.png"
#set out "wifi-default.eps"
#set title "Percentage over Time"
set ylabel "Percentage (%)"
set yrange [0:100]
set xlabel "Time (s)"
set xrange [50:400]
set style data lines

plot "delay.txt" using 1:(($0+1)*100/1001)  with lines lt 1 title "1 Producer",\
 "delay1.txt" using 1:(($0+1)*100/920)  with lines lt 3 title  "3 Producers"
// "node_10_n_4_5.txt" with lines lt 4 title "4 Producers",\
 //"node_10_n_4_6.txt" with lines lt 5 title "5 Producers",\
 //"node_10_n_4_7.txt" with lines lt 6 title "6 Producers",\
 //"node_10_n_4_8.txt" with lines lt 7 title "7 Producers",\
 //"node_10_n_4_9.txt" with lines lt 8 title "8 Producers",\
 //"node_10_n_4_10.txt" with lines lt 9 title "9 Producers"
