#!/bin/bash

n=9
for s in 9 7 5 3 1
do
  for t in 1000 3000 5000
  do
    echo "----------------" "$n""_""$s""_""$t"
    python3 ~/Honours/oppt/tools/generateCuttingV2CFG.py -n $n -s $s -t $t -o ~/Honours/oppt/runsexact/rs2/batch_1/cfgs/"$n"_"$s"_"$t"-CuttingV2.cfg 
    ~/Honours/ABTLiteOriginal/bin/./abtLite --cfg ~/Honours/oppt/runsexact/rs2/batch_1/cfgs/"$n"_"$s"_"$t"-CuttingV2.cfg > ~/Honours/oppt/runsexact/rs2/batch_1/"$n"_"$s"_"$t".txt
    python3 ~/Honours/oppt/tools/extract_stats.py -n $n -s $s -t $t -i ~/Honours/oppt/runsexact/rs2/batch_1/"$n"_"$s"_"$t".txt -o ~/Honours/oppt/runsexact/rs2/batch_1/run_results_2.csv
  done
done