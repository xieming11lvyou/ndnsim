#!/bin/bash --login

PRODUCERS_NUM="11 16 21"
#TRIALS="1 2 3 4 5"

#for trial in $TRIALS
#do




for producer in $PRODUCERS_NUM
do
  echo producer $producer
  str1="NODE100_rIP_3_"
  a=1
  str2=`expr $producer - $a`
  str2="$str2"
  echo $str2
  filename=$str1$str2
  echo $filename
  ./waf --run "first --n=$producer --m=$producer" >$filename.txt
done
#done

#
#Another SQL command which just collects raw numbers of frames receved.
#
#CMD="select Experiments.input,avg(Singletons.value) \
#    from Singletons,Experiments \
#    where Singletons.run = Experiments.run AND \
#          Singletons.name='wifi-rx-frames' \
#    group by Experiments.input \
#    order by abs(Experiments.input) ASC;"

# mv ../../data.db .

# CMD="select exp.input,avg(100-((rx.value*100)/tx.value)) \
#     from Singletons rx, Singletons tx, Experiments exp \
#     where rx.run = tx.run AND \
#           rx.run = exp.run AND \
#           rx.variable='receiver-rx-packets' AND \
#           tx.variable='sender-tx-packets' \
#     group by exp.input \
#     order by abs(exp.input) ASC;"

# sqlite3 -noheader data.db "$CMD" > wifi-default.data
# sed -i.bak "s/|/   /" wifi-default.data
# rm wifi-default.data.bak
# gnuplot wifi-example.gnuplot

# echo "Done; data in wifi-default.data, plot in wifi-default.eps"
