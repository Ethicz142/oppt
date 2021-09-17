import csv
import getopt
import os
import sys
import pathlib

INPUT = None 

argument_list = sys.argv[1:]
short_options = "i:"
long_options = ["input="]

try:
  arguments, values = getopt.getopt(argument_list, short_options, long_options)
  for current_argument, current_value in arguments:
    if current_argument in ("-i", "--input"):
      INPUT = str(current_value)
except getopt.error as err:
  # Output error, and return with an error code
  print (str(err))
  sys.exit(2)
print(INPUT)
if INPUT is None:
  print ("Please define: -i (input path)")
  sys.exit(2)

# DIR_PATH = os.path.dirname(os.path.realpath(__file__))
# RUN_PATH = DIR_PATH + f"/../runs/{REWARDS_SCHEMA}/{NUMBER_OF_CUTTERS}_{NUMBER_OF_SUITABLE_CUTTERS}/" + FILE_NAME

# https://stackoverflow.com/a/52546064/15754796

def split_logs(input_file_path):
  input_file_path = pathlib.Path(input_file_path)
  output_folder_path = input_file_path.parent

  run_num = 0
  output_file_path = output_folder_path / (str(input_file_path.stem) + "header.txt")
  
  with open(input_file_path, 'r') as fh:
    fh_new = open(output_file_path, 'w')
    line = fh.readline()
    while line:
      if "Run #" in line:
        fh_new.close()
        run_num += 1
        output_file_path = output_folder_path / (str(input_file_path.stem) + f"_{run_num}.txt")
        print (output_file_path)
        fh_new = open(output_file_path, 'w')
        fh_new.write(line)
      else:
        fh_new.write(line)

      line = fh.readline()
    fh_new.close()

split_logs(INPUT)


# discounted_rewards = []
# number_of_actions = []
# final_object_states = []
# exact_num_of_suitable_cutters = 0;

# num_actions_in_run = 0
# object_state = None
# for line in lines:
#   if 'Run #' in line: 
#     num_actions_in_run = 0
#     object_state = None
#   elif 'action:' in line:
#     num_actions_in_run += 1
#   elif 'Next state:' in line:
#     #Next state: 0 0.513467 0.858946 0.591451 0.696497 0.585992 0.885826 w: 1
#     object_state = line.split(' ')[2]
#   elif 'Discounted reward:' in line:
#     number_of_actions.append(num_actions_in_run)
#     final_object_states.append(object_state)
#     num_actions_in_run = 0
#     object_state = None
#     discounted_rewards.append(float(line.split(' ')[2]))
#   elif 'Initial state:' in line:
#     #looks like this: Initial state: 0 0.594173 0.815712 0.314861 0.584668 0.546172 0.891896 w: 1
#     stateValues = line.split(' ')[3:-2];
#     ns = 0
#     for i in range(0, len(stateValues), 2):
#       hardness = float(stateValues[i])
#       sharpness = float(stateValues[i+1])
#       if hardness >= trueObjectHardnessRange[0] and hardness <= trueObjectHardnessRange[1] and sharpness >= trueObjectSharpnessRange[0] and sharpness <= trueObjectSharpnessRange[1]:
#         ns += 1
#     if ns == NUMBER_OF_SUITABLE_CUTTERS:
#       exact_num_of_suitable_cutters += 1


# # print(discounted_rewards)
# mean, lower_interval, upper_interval = mean_confidence_interval(discounted_rewards)
# # print(mean_confidence_interval(discounted_rewards))
# # print(len(number_of_actions), number_of_actions, sum(number_of_actions))
# # print(mean_confidence_interval(number_of_actions))
# mean_a, lower_a, upper_a = mean_confidence_interval(number_of_actions)
# # print(len(final_object_states), final_object_states, number_of_actions.count(1)/len(final_object_states))

# # with open(DIR_PATH + f"/../runs/{REWARDS_SCHEMA}/" + "run_results.csv", "a+", encoding='UTF8', newline='') as f:
# #   writer = csv.writer(f)
# #   writer.writerow([f"{NUMBER_OF_CUTTERS}_{NUMBER_OF_SUITABLE_CUTTERS}", mean, lower_interval, upper_interval, discounted_rewards])
# file_exists = os.path.isfile(OUTPUT)

# with open (OUTPUT, 'a') as csvfile:
#   headers = ['number of cutters', 'minimum number of suitable cutters', 'runs', 'a %', 'time',
#     'mean discounted reward', 'lower discounted reward', 'upper discounted reward', 
#     'mean num actions', 'lower num actions', 'upper num actions', 'cut %', 'exact num of suitable cutters %']
#   writer = csv.DictWriter(csvfile, delimiter=',', lineterminator='\n',fieldnames=headers)

#   if not file_exists:
#       writer.writeheader()  # file doesn't exist yet, write a header

#   # writer.writerow({'TimeStamp': , 'light': dic['light'], 'Proximity': dic['prox']})
#   writer.writerow({
#     'number of cutters': NUMBER_OF_CUTTERS,
#     'minimum number of suitable cutters': NUMBER_OF_SUITABLE_CUTTERS,
#     'runs': len(discounted_rewards),
#     'a %' : A_VALUE,
#     'time': TIME,
#     'mean discounted reward': mean,
#     'lower discounted reward': lower_interval,
#     'upper discounted reward': upper_interval,
#     'mean num actions' :mean_a, 
#     'lower num actions': lower_a, 
#     'upper num actions': upper_a, 
#     'cut %': final_object_states.count("1")/len(final_object_states),
#     'exact num of suitable cutters %': exact_num_of_suitable_cutters/len(final_object_states)
#   })
