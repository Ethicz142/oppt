#!/bin/bash

s=2
for t in 1000 3000 5000
do
  for n in 3 5 7 9 11
  do
    for a in 25 50 75
    do
      echo "----------------" "$n""_""$s""_""$a""_""$t"
      python3 ~/Honours/oppt/tools/generateCuttingV2CFG.py -n $n -s $s -a $a -t $t -o ~/Honours/oppt/runsalgo/rs2/batch_1/cfgs/"$n"_"$s"_"$a"_"$t"-CuttingV2.cfg 
      ~/Honours/ABTLite/bin/./abtLite --cfg ~/Honours/oppt/runsalgo/rs2/batch_1/cfgs/"$n"_"$s"_"$a"_"$t"-CuttingV2.cfg > ~/Honours/oppt/runsalgo/rs2/batch_1/"$n"_"$s"_"$a"_"$t".txt
    done
  done
done