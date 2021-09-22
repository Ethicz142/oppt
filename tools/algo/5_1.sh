#!/bin/bash

n=5
for t in 1000
do
  for s in 1
  do
    for a in 50
    do
      echo "----------------" "$n""_""$s""_""$a""_""$t"
      python3 ~/Honours/oppt/tools/generateCuttingV2CFG.py -n $n -s $s -a $a -t $t -o ~/Honours/oppt/runsalgo/rs3/test/cfgs/"$n"_"$s"_"$a"_"$t"-CuttingV2.cfg 
      ~/Honours/ABTLite/bin/./abtLite --cfg ~/Honours/oppt/runsalgo/rs3/test/cfgs/"$n"_"$s"_"$a"_"$t"-CuttingV2.cfg > ~/Honours/oppt/runsalgo/rs3/test/"$n"_"$s"_"$a"_"$t".txt
    done
  done
done