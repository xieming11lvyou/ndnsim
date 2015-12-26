set terminal postscript portrait enhanced lw 2 "Helvetica" 14

set size 2,1

#-------------------------------------------------------

set out "Interest.eps"
set style histogram clustered gap 2
set style data histograms
set xtics border in scale 1,0.5 nomirror rotate by 0 offset character 0,0,0
set boxwidth 1
set ylabel "Number of Sent Interest Packets" font "Times-Roman,20"
set xlabel "Object Size" font "Times-Roman,20"
set style fill solid 0.4
#set style fill pattern 3 border -1


plot "a.data" using 2:xticlabels(1) lt 1 lc rgb "orange-red" title columnheader(2),''using 3:xticlabels(1) lt 1 lc rgb "light-green" title columnheader(3),''using 4:xticlabels(1) lt 1 lc rgb "purple"title columnheader(4)