#!/bin/bash

for t in 1000 3000 5000
do
  for n in 3 5 7 9 11
  do
    echo "----------------" "$n""_""2""_""$t"
    python3 ~/Honours/oppt/tools/generateCuttingV2CFG.py -n $n -s 2 -t $t -o ~/Honours/oppt/runsexact/rs3/batch_2/cfgs/"$n"_2_"$t"-CuttingV2.cfg 
    ~/Honours/ABTLiteOriginal/bin/./abtLite --cfg ~/Honours/oppt/runsexact/rs3/batch_2/cfgs/"$n"_2_"$t"-CuttingV2.cfg > ~/Honours/oppt/runsexact/rs3/batch_2/"$n"_2_"$t".txt
  done
done