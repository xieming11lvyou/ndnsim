#!/bin/sh
./waf --run "scratch/ndn-another-test --T=100 --p=2 --num=20 --dec=2" >20_newtemp2.txt
./waf --run "scratch/ndn-another-test --T=100 --p=2 --num=30 --dec=1" >30_newtemp2.txt
./waf --run "scratch/ndn-another-test --T=100 --p=2 --num=40 --dec=0.5" >40_newtemp2.txt
./waf --run "scratch/ndn-another-test --T=100 --p=2 --num=50 --dec=0.4" >50_newtemp2.txt
./waf --run "scratch/ndn-another-test --T=100 --p=2 --num=60 --dec=0.3" >60_newtemp2.txt
num=70
while [ $num -le 100 ]
do
	echo $num
#./waf --run "scratch/ndn-origin-test --T=10  --num=$num" >${num}_newtemp1.txt
#./waf --run "scratch/ndn-3-test --T=10 --p=2 --num=$num --dec=1" >${num}_newtemp3.txt
./waf --run "scratch/ndn-another-test --T=100 --p=2 --num=$num --dec=0.1" >${num}_newtemp2.txt
num=$(($num+10))
done


num=20
while [ $num -le 100 ]
do
	echo $num
#./waf --run "scratch/ndn-origin-test --T=100  --num=$num" >${num}_newtemp1.txt
./waf --run "scratch/ndn-3-test --T=100 --p=2 --num=$num --dec=1" >${num}_newtemp3.txt
#./waf --run "scratch/ndn-another-test --T=100 --p=2 --num=$num --dec=2" >${num}_newtemp2.txt
num=$(($num+10))
done