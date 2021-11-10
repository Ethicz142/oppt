import multiprocessing as mp
import os.path as osp
import pathlib
import subprocess
from dataclasses import dataclass
from subprocess import PIPE, STDOUT
from typing import List, Optional
import itertools

from generateCuttingV2CFG import generate_cutting_V2_cfg


@dataclass
class ExpConfig:
  n_list: List[int]
  s_list: List[int] 
  a_list: List[int]
  t_list: List[int]
  r: int
  trials: int

def create_cfgs(config: ExpConfig, path):
  for t in config.t_list:
    for n in config.n_list:
      for s in config.s_list:
          for a in config.a_list:
            for trial_num in range(1, config.trials + 1):
              cfg_path = pathlib.Path(path, f"n-{n}_s-{s}_a-{a}_t-{t}")
              cfg_path.mkdir(parents=True, exist_ok=True)
              print(f"n-{n}_s-{s}_a-{a}_t-{t}")
              print(cfg_path.parent.name)
              generate_cutting_V2_cfg(cfg_path / f"{trial_num}.cfg", n, s, t, config.r, a, f"{cfg_path.parent.name}/{cfg_path.name}/{trial_num}")

def run_solver(solver_path, cfg_path):
  print (str(cfg_path).rsplit('.', 1)[0] + '.txt')
  f = open(str(cfg_path).rsplit('.', 1)[0] + '.txt' , "w+")
  subprocess.call(f"{solver_path} --cfg {cfg_path}", stdout=f, stderr=STDOUT, encoding='utf-8', shell=True)
  # print (solver_path, cfg_path)

def run_experiments_original_algo(orig_config: Optional[ExpConfig], algo_config: Optional[ExpConfig], path, num_processes):
  def create_combinations(config: ExpConfig, solver_path, parent_path): 
    combinations = [(solver_path, pathlib.Path(parent_path, f"n-{n}_s-{s}_a-{a}_t-{t}", f"{tn}.cfg")) for n, s, a, t, tn in
      itertools.product(config.n_list, config.s_list, config.a_list, config.t_list, range(1, config.trials + 1))]
    return combinations

  original_path = None if orig_config is None else pathlib.Path(path, "original")
  algo_path = None if algo_config is None else pathlib.Path(path, "algo")

  combinations = []

  if orig_config is not None:
    create_cfgs(orig_config, original_path)
    combinations.extend(create_combinations(orig_config, f"/home/{pathlib.Path(path).parts[2]}/Honours/ABTLiteOriginal/bin/abtLite", original_path))
    print (pathlib.Path(path).parts)
    
    
  if algo_config is not None: 
    create_cfgs(algo_config, algo_path)
    combinations.extend(create_combinations(algo_config, f"/home/{pathlib.Path(path).parts[2]}/Honours/ABTLite/bin/abtLite", algo_path))

  if num_processes == 1:
    for solver_path, cfg_path in combinations:
      run_solver(solver_path, cfg_path)
  else:
      with mp.Pool(num_processes) as p:
          p.starmap(run_solver, combinations)

if __name__ == "__main__":
  run_for_a_config = ExpConfig(
    n_list = [10],
    s_list = [1],
    a_list = [25, 50, 75],
    t_list = [1000, 3000],
    r = 30,
    trials = 3
  )
  orig_config_constant_n = ExpConfig(
    n_list = [20],
    s_list = [7,5,3,1],
    a_list = [100],
    t_list = [1000, 3000],
    r = 50,
    trials = 20
  )
  orig_config_constant_s = ExpConfig(
    n_list = [5, 10, 15, 20],
    s_list = [1],
    a_list = [100],
    t_list = [1000, 3000],
    r = 200,
    trials = 20
  )
  algo_config_constant_n = ExpConfig(
    n_list = [20],
    s_list = [7,5,3,1],
    a_list = [50],
    t_list = [1000, 3000],
    r = 50,
    trials = 20
  )
  algo_config_constant_s = ExpConfig(
    n_list = [5, 10, 15, 20],
    s_list = [1],
    a_list = [50],
    t_list = [1000, 3000],
    r = 200,
    trials = 20
  )
  orig_config_50_1 = ExpConfig(
    n_list = [50],
    s_list = [1],
    a_list = [100],
    t_list = [1000, 3000, 5000],
    r = 50,
    trials = 20
  )
  algo_config_50_1 = ExpConfig(
    n_list = [50],
    s_list = [1],
    a_list = [50],
    t_list = [1000, 3000, 5000],
    r = 50,
    trials = 20
  )
  orig_config_large_constant_s = ExpConfig(
    n_list = [60,70,80,90,100],
    s_list = [1],
    a_list = [100],
    t_list = [1000, 3000],
    r = 50,
    trials = 20
  )
  algo_config_large_constant_s = ExpConfig(
    n_list = [60,70,80,90,100],
    s_list = [1],
    a_list = [50],
    t_list = [1000, 3000],
    r = 50,
    trials = 20
  )
  orig_config_20_50_1 = ExpConfig(
    n_list = [20, 50],
    s_list = [1],
    a_list = [100],
    t_list = [1000, 3000],
    r = 50,
    trials = 20
  )
  algo_config_20_50_1 = ExpConfig(
    n_list = [20, 50],
    s_list = [1],
    a_list = [50],
    t_list = [1000, 3000],
    r = 50,
    trials = 20
  )
  orig_config_n_30_40_50 = ExpConfig(
    n_list = [30, 40, 50],
    s_list = [1],
    a_list = [100],
    t_list = [1000, 3000],
    r = 200,
    trials = 20
  )
  algo_config_n_30_40_50 = ExpConfig(
    n_list = [30, 40, 50],
    s_list = [1],
    a_list = [50],
    t_list = [1000, 3000],
    r = 200,
    trials = 20
  )
  orig_config_n_40_s_3_5_7 = ExpConfig(
    n_list = [40],
    s_list = [7, 5, 3],
    a_list = [100],
    t_list = [1000, 3000],
    r = 200,
    trials = 20
  )
  algo_config_n_40_s_3_5_7 = ExpConfig(
    n_list = [40],
    s_list = [7, 5, 3],
    a_list = [50],
    t_list = [1000, 3000],
    r = 200,
    trials = 20
  )
  run_experiments_original_algo(orig_config_n_40_s_3_5_7, algo_config_n_40_s_3_5_7, "/home/ethanNguyen/Honours/oppt/runsv3-3", 8)
  print('experiments are completed.')

