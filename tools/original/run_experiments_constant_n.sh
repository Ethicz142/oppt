#!/bin/bash

n=9

for t in 1000 3000 5000
do
  for s in 9 7 5 3 1
  do
    echo "----------------" "$n""_""$s""_""$t"
    python3 ~/Honours/oppt/tools/generateCuttingV2CFG.py -n $n -s $s -t $t -o ~/Honours/oppt/runsexact/rs3/batch_2/cfgs/"$n"_"$s"_"$t"-CuttingV2.cfg 
    ~/Honours/ABTLiteOriginal/bin/./abtLite --cfg ~/Honours/oppt/runsexact/rs3/batch_2/cfgs/"$n"_"$s"_"$t"-CuttingV2.cfg > ~/Honours/oppt/runsexact/rs3/batch_2/"$n"_"$s"_"$t".txt
  done
done