#!/bin/bash

for n in 3 5 7 9 11
do
  for t in 1000 3000 5000 10000
  do
    echo "----------------" "$n""_""2""_""$t"
    python3 ~/Honours/oppt/tools/extract_stats.py -n $n -s 2 -t $t -i ~/Honours/oppt/runs/rs2/batch_1/"$n"_2_"$t".txt -o ~/Honours/oppt/runs/rs2/batch_1/run_results_stats.csv
  done
done

n=9
for s in 9 7 5 3 1
do
  for t in 1000 3000 5000 10000
  do
    echo "----------------" "$n""_""$s""_""$t"
    python3 ~/Honours/oppt/tools/extract_stats.py -n $n -s $s -t $t -i ~/Honours/oppt/runs/rs2/batch_1/"$n"_"$s"_"$t".txt -o ~/Honours/oppt/runs/rs2/batch_1/run_results_stats_2.csv
  done
done