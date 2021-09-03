#!/bin/bash

for n in 3 5 7 9 11
do
  for t in 10000
  do
    echo "----------------" "$n""_""2""_""$t"
    python3 ~/Honours/oppt/tools/generateCuttingV2CFG.py -n $n -s 2 -t $t -o ~/Honours/oppt/runsexact/rs2/batch_1/cfgs/"$n"_2_"$t"-CuttingV2.cfg 
    ~/Honours/ABTLiteOriginal/bin/./abtLite --cfg ~/Honours/oppt/runsexact/rs2/batch_1/cfgs/"$n"_2_"$t"-CuttingV2.cfg > ~/Honours/oppt/runsexact/rs2/batch_1/"$n"_2_"$t".txt
    python3 ~/Honours/oppt/tools/extract_stats.py -n $n -s 2 -t $t -i ~/Honours/oppt/runsexact/rs2/batch_1/"$n"_2_"$t".txt -o ~/Honours/oppt/runsexact/rs2/batch_1/run_results.csv
  done
done