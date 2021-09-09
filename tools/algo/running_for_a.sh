#!/bin/bash

n=9
for t in 1000 3000 5000
do
  for s in 1
  do
    for a in 20 40 60 80
    do
      echo "----------------" "$n""_""$s""_""$a""_""$t"
      python3 ~/Honours/oppt/tools/generateCuttingV2CFG.py -n $n -s $s -a $a -t $t -o ~/Honours/oppt/runsalgo/rs2/finding_a/cfgs/"$n"_"$s"_"$a"_"$t"-CuttingV2.cfg 
      ~/Honours/ABTLite/bin/./abtLite --cfg ~/Honours/oppt/runsalgo/rs2/finding_a/cfgs/"$n"_"$s"_"$a"_"$t"-CuttingV2.cfg > ~/Honours/oppt/runsalgo/rs2/finding_a/"$n"_"$s"_"$a"_"$t".txt
    done
  done
done