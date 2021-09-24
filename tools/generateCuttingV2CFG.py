import random, getopt, sys

NUMBER_OF_CUTTERS = None
NUMBER_OF_SUITABLE_CUTTERS = None
A_VALUE = 100
STEP_TIMEOUT = None
OUTPUT = None
RUNS = 500

argument_list = sys.argv[1:]
short_options = "hn:s:a:t:r:o:"
long_options = ["help", "number=", "suitable=","a=", "time=","runs", "output="]

try:
  arguments, values = getopt.getopt(argument_list, short_options, long_options)
  for current_argument, current_value in arguments:
    if current_argument in ("-h", "--help"):
      print ("-n (Number of cutters) -s (Number of suitable cutters)")
    elif current_argument in ("-n", "--number"):
      NUMBER_OF_CUTTERS = int(current_value)
    elif current_argument in ("-s", "--suitable"):
      NUMBER_OF_SUITABLE_CUTTERS = int(current_value)
    elif current_argument in ("-a", "--a"):
      A_VALUE = int(current_value)
    elif current_argument in ("-t", "--time"):
      STEP_TIMEOUT = int(current_value) #in ms
    elif current_argument in ("-o", "--output"):
      OUTPUT = str(current_value) 
    elif current_argument in ("-r", "--runs"):
      RUNS = int(current_value) 
except getopt.error as err:
  # Output error, and return with an error code
  print (str(err))
  sys.exit(2)

if NUMBER_OF_CUTTERS is None or NUMBER_OF_SUITABLE_CUTTERS is None or STEP_TIMEOUT is None:
  print ("Please define: -n (Number of cutters) -s (Number of suitable cutters) -t (Step timeout)")
  sys.exit(2)

with open('modelCuttingV2.cfg', 'r') as reader:
  lines = reader.readlines()

with open(OUTPUT, 'w') as writer:
  in_State = False
  in_Action = False
  in_Observation = False

  for line in lines:
    modified_line = line 

    if 'numberOfCutters' in line:
      modified_line = f'numberOfCutters = {NUMBER_OF_CUTTERS}\n'
    elif 'numberOfSuitableCutters' in line: 
      modified_line = f'numberOfSuitableCutters = {NUMBER_OF_SUITABLE_CUTTERS}\n'

    elif 'logPath' in line:
      modified_line = f'logPath = log/{NUMBER_OF_CUTTERS}_{NUMBER_OF_SUITABLE_CUTTERS}_{A_VALUE}_{STEP_TIMEOUT}\n'

    elif 'stepTimeout' in line:
      modified_line = f'stepTimeout = {STEP_TIMEOUT}\n'
    
    elif 'a =' in line:
      modified_line = f'a = {A_VALUE/100}\n'
    
    elif 'nRuns =' in line:
      modified_line = f'nRuns = {RUNS}\n'

    elif in_State and 'additionalDimensions' in line: 
      modified_line = f'additionalDimensions = {NUMBER_OF_CUTTERS * 2 + 1}\n'
    elif in_State and 'additionalDimensionLimits' in line: 
      modified_line = 'additionalDimensionLimits = [[0, 2]' + ', [0, 1]' * NUMBER_OF_CUTTERS * 2 + ']\n'
      in_State = False

    elif in_Action and 'additionalDimensionLimits' in line: 
      modified_line = f'additionalDimensionLimits = [[-1, {NUMBER_OF_CUTTERS}]]\n'
      in_Action = False

    elif in_Observation and 'additionalDimensions' in line: 
      modified_line = f'additionalDimensions = {NUMBER_OF_CUTTERS * 2 + 1}\n'
    elif in_Observation and 'additionalDimensionLimits' in line: 
      modified_line = 'additionalDimensionLimits = [[0, 9]' + ', [0, 3]' * NUMBER_OF_CUTTERS * 2 + ']\n'
      in_Observation = False

    elif 'numInputStepsActions' in line: 
      modified_line = f'numInputStepsActions = {NUMBER_OF_CUTTERS + 2}\n'

    if '[state]' in line:
      in_State = True
    elif '[action]' in line:
      in_Action = True
    elif '[observation]' in line:
      in_Observation = True

    writer.write(modified_line)
