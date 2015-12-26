#!/bin/sh
num=20
while [ $num -le 100 ]
do
	echo $num
#./waf --run "scratch/ndn-origin-test --T=100  --num=$num" >${num}_newtemp1.txt
./waf --run "scratch/ndn-3-test --T=200 --p=2 --num=$num --dec=1" >${num}_newtemp4.txt
#./waf --run "scratch/ndn-another-test --T=100 --p=2 --num=$num --dec=2" >${num}_newtemp2.txt
num=$(($num+10))
done