#!/bin/bash

gnuplot  <<  EOF  
set terminal png
set output "thout.png"
set grid

#plot sin(x)

plot "thout.txt" using 1:2 title "thoughout" with l lt 1 lw 3 lc rgb "orange-red"

quit
EOF



