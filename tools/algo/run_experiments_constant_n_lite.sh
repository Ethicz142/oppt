#!/bin/bash

n=9
for t in 1000
do
  for s in 9 7 5 3 1
  do
    for a in 25 50 75
    do
      echo "----------------" "$n""_""$s""_""$a""_""$t"
      python3 ~/Honours/oppt/tools/generateCuttingV2CFG.py -n $n -s $s -a $a -t $t -o ~/Honours/oppt/runsalgo/rs2/batch_1/cfgs/"$n"_"$s"_"$a"_"$t"-CuttingV2.cfg 
      ~/Honours/ABTLite/bin/./abtLite --cfg ~/Honours/oppt/runsalgo/rs2/batch_1/cfgs/"$n"_"$s"_"$a"_"$t"-CuttingV2.cfg > ~/Honours/oppt/runsalgo/rs2/batch_1/"$n"_"$s"_"$a"_"$t".txt
      python3 ~/Honours/oppt/tools/extract_stats.py -n $n -s $s -a $a -t $t -i ~/Honours/oppt/runsalgo/rs2/batch_1/"$n"_"$s"_"$a"_"$t".txt -o ~/Honours/oppt/runsalgo/rs2/batch_1/run_results_2.csv
    done
  done
done