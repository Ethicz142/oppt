import csv
import getopt
import os
import sys

import numpy as np
import scipy.stats as st


def mean_confidence_interval(data, confidence=0.95):
  a = 1.0 * np.array(data)
  m = np.mean(a)
  if len(a) <= 30:
    li, ui = st.t.interval(confidence, len(a)-1, loc=m, scale=st.sem(a))
    return m, li, ui
  else:
    li, ui = st.norm.interval(confidence, loc=m, scale=st.sem(a))
    return m, li, ui

REWARDS_SCHEMA = 'rs1'

FILE_NAME = None

argument_list = sys.argv[1:]
short_options = "hf:n:s:"
long_options = ["help", "filename=", "number=", "suitable="]

try:
  arguments, values = getopt.getopt(argument_list, short_options, long_options)
  for current_argument, current_value in arguments:
    if current_argument in ("-h", "--help"):
      print ("-f (filename)")
    elif current_argument in ("-f", "--filename"):
      FILE_NAME = str(current_value)
    elif current_argument in ("-n", "--number"):
      NUMBER_OF_CUTTERS = int(current_value)
    elif current_argument in ("-s", "--suitable"):
      NUMBER_OF_SUITABLE_CUTTERS = int(current_value)
except getopt.error as err:
  # Output error, and return with an error code
  print (str(err))
  sys.exit(2)

if FILE_NAME is None or NUMBER_OF_CUTTERS is None or NUMBER_OF_SUITABLE_CUTTERS is None:
  print ("Please define: -f (filename) -n (number of cutters) - s(number of suitable cutters")
  sys.exit(2)

DIR_PATH = os.path.dirname(os.path.realpath(__file__))
RUN_PATH = DIR_PATH + f"/../runs/{REWARDS_SCHEMA}/{NUMBER_OF_CUTTERS}_{NUMBER_OF_SUITABLE_CUTTERS}/" + FILE_NAME

with open(RUN_PATH, 'r') as reader:
  lines = reader.readlines()

if not lines:
    print ("Can't find file")
    sys.exit(2)

discounted_rewards = []

for line in lines:
  if 'Discounted reward:' in line:
    discounted_rewards.append(float(line.split(' ')[2]))

print(discounted_rewards)
mean, lower_interval, upper_interval = mean_confidence_interval(discounted_rewards)
print(mean_confidence_interval(discounted_rewards))

with open(DIR_PATH + f"/../runs/{REWARDS_SCHEMA}/" + "run_results.csv", "a+", encoding='UTF8', newline='') as f:
  writer = csv.writer(f)
  writer.writerow([f"{NUMBER_OF_CUTTERS}_{NUMBER_OF_SUITABLE_CUTTERS}", mean, lower_interval, upper_interval, discounted_rewards])
