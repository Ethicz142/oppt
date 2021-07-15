import getopt
import os
import sys

import numpy as np
import scipy.stats as st

def mean_confidence_interval(data, confidence=0.95):
  a = 1.0 * np.array(data)
  if len(a) <= 30:
    return st.t.interval(confidence, len(a)-1, loc=np.mean(a), scale=st.sem(a))
  else:
    return st.norm.interval(confidence, loc=np.mean(a), scale=st.sem(a))


FILE_NAME = None

argument_list = sys.argv[1:]
short_options = "hf:"
long_options = ["help", "filename="]

try:
  arguments, values = getopt.getopt(argument_list, short_options, long_options)
  for current_argument, current_value in arguments:
    if current_argument in ("-h", "--help"):
      print ("-f (filename)")
    elif current_argument in ("-f", "--filename"):
      FILE_NAME = str(current_value)
except getopt.error as err:
  # Output error, and return with an error code
  print (str(err))
  sys.exit(2)

if FILE_NAME is None:
  print ("Please define: -f (filename)")
  sys.exit(2)

DIR_PATH = os.path.dirname(os.path.realpath(__file__))
RUN_PATH = DIR_PATH + "/../runs/rs1/" + FILE_NAME

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
print(mean_confidence_interval(discounted_rewards))