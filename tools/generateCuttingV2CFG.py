import random

NUMBER_OF_CUTTERS = 5

with open('modelCuttingV2.cfg', 'r') as reader:
  lines = reader.readlines()

with open('CuttingV2.cfg', 'w') as writer:
  in_State = False
  in_Action = False
  in_Observation = False

  for line in lines:
    modified_line = line 

    if 'numberOfCutters' in line:
      modified_line = f'numberOfCutters = {NUMBER_OF_CUTTERS}\n'
    elif 'trueCutterProperties' in line:
      modified_line = 'trueCutterProperties = ['
      modified_line += ', '.join([str(round(random.uniform(0,1), 2)) for _ in range(NUMBER_OF_CUTTERS * 2)])
      modified_line += ']\n'

    elif in_State and 'additionalDimensions' in line: 
      modified_line = f'additionalDimensions = {NUMBER_OF_CUTTERS * 2 + 1}\n'
    elif in_State and 'additionalDimensionLimits' in line: 
      modified_line = 'additionalDimensionLimits = [[0, 2]' + ', [0, 1]' * NUMBER_OF_CUTTERS * 2 + ']\n'
      in_State = False

    elif in_Action and 'additionalDimensionLimits' in line: 
      modified_line = f'additionalDimensionLimits = [[0, {NUMBER_OF_CUTTERS}]]\n'
      in_Action = False

    elif in_Observation and 'additionalDimensions' in line: 
      modified_line = f'additionalDimensions = {NUMBER_OF_CUTTERS * 2 + 1}\n'
    elif in_Observation and 'additionalDimensionLimits' in line: 
      modified_line = 'additionalDimensionLimits = [[0, 100]' + ', [0, 1]' * NUMBER_OF_CUTTERS * 2 + ']\n'
      in_Observation = False

    elif 'numInputStepsActions' in line: 
      modified_line = f'numInputStepsActions = {NUMBER_OF_CUTTERS + 1}\n'

    if '[state]' in line:
      in_State = True
    elif '[action]' in line:
      in_Action = True
    elif '[observation]' in line:
      in_Observation = True

    writer.write(modified_line)
