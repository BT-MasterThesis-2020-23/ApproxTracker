import os
import string
import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl
import pandas as pd
from exportExcel import exportExcel

# > 250 kernel
#"R_EULER3D", "R_NEEDLE", "R_SRAD_V1",

cuda_applications = ["G_BC_LINEAR_BASE", "G_BC_LINEAR_LB", "G_BC_TOPO_BASE", "G_BC_TOPO_LB", "G_BFS", "G_CC_AFFOREST",
  "G_CC_BASE", "G_CC_WARP", "G_MST", "G_PR_BASE", "G_PR_WARP", "G_SPMV", "G_SPMV_VECTOR", "G_SPMV_WARP", 
  "G_SSSP_LINEAR_BASE", "G_SSSP_LINEAR_LB", "P_ATAX", "P_CORR", "R_HYBRIDSORT", "T_LSTM"]

# cuda_applications = ["G_BC_LINEAR_BASE", "G_BC_LINEAR_LB", "G_BC_TOPO_BASE", "G_BC_TOPO_LB", "G_BFS", 
#  "G_CC_AFFOREST", "G_CC_BASE", "G_CC_WARP", "G_MST", "G_PR_BASE", "G_PR_WARP", "G_SPMV", 
#  "G_SPMV_VECTOR", "G_SPMV_WARP", "G_SSSP_LINEAR_BASE", "G_SSSP_LINEAR_LB", "P_ATAX", "P_CORR", "R_BACKPROP", 
#  "R_BFS", "R_GAUSSIAN", "R_HEARTWALL", "R_HOTSPOT", "R_HYBRIDSORT", "R_HYBRIDSORT_2", "R_LUD", 
#  "R_NN", "R_PATHFINDER",  "R_SRAD_V2", "R_STREAMCLUSTER", "T_ALEXNET", "T_CIFARNET", 
#  "T_GRU", "T_LSTM", "T_LSTM_2", "T_RESNET", "T_SQUEEZENET"]

def openAppTest(app):
  resultantFiles = list(app.keys())
  for i in resultantFiles:
    file = open(i, 'r')
    file.readlines()
    file.close()

def countExperimentsAndNames(foldername):
  app = {}
  for file in os.listdir(foldername):
    d = os.path.join(foldername, file)
    if os.path.isdir(d):
      if 'G_' in d:
        app[foldername + '/' + d[len(foldername)+1:] + '/' + d[len(foldername)+1:] + '.txt'] = {}
      else:
        app[foldername + '/' + foldername[2:].lower() + '.txt'] = {} 
  return app

def countNofKernels(app):
  resultantFiles = list(app.keys())
  for i in resultantFiles:
    file = open(i, 'r')
    nofLines = len(file.readlines())
    file.close()

    file = open(i, 'r')
    count = 0
    for j in range (0, nofLines):
      line = file.readline()
      if "gpgpu_simulation_time =" in line:
        count += 1
    app[i]['nofKernels'] = int(count)
    file.close()
  return app

def collectPerKernelMetrics(metrics):
  collectedMetrics = {}
  collectedMetrics["rowBufLoc"] = 0
  for i in metrics:
    if "gpu_sim_cycle" in i:
      collectedMetrics["cycle"] = int(i[len("gpu_sim_cycle = "):len(i)-1])
    elif "gpu_sim_insn" in i:
      collectedMetrics["instruct"] = int(i[len("gpu_sim_insn = "):len(i)-1])
    elif "gpu_ipc =" in i:
      offset = len("gpu_ipc = ")
      while True:
        if i[offset] == ' ':
          offset += 1
        else:
          break
      collectedMetrics["gpuIpc"] = float(i[offset:len(i)-1])  
    elif "gpu_occupancy =" in i:
      collectedMetrics["occup"] = float(i[len("gpu_occupancy = "):len(i)-3])
    elif "gpu_stall_dramfull =" in i:
      collectedMetrics["stallDramFull"] = float(i[len("gpu_stall_dramfull = "):len(i)-1])
    elif "L2_BW  =" in i:
      offset = len("L2_BW  = ")
      while True:
        if i[offset] == ' ':
          offset += 1
        else:
          break
      collectedMetrics["L2_BW"] = float(i[offset:len(i) - len(" GB/Sec") - 1])  
    elif "partiton_level_parallism =" in i:
      offset = len("partiton_level_parallism =")
      while True:
        if i[offset] == ' ':
          offset += 1
        else:
          break
      collectedMetrics["partLevelPar"] = float(i[offset:len(i) - 1])  
    elif "partiton_level_parallism_util =" in i:
      offset = len("partiton_level_parallism_util =")
      while True:
        if i[offset] == ' ':
          offset += 1
        else:
          break
      collectedMetrics["partLevelParUtil"] = float(i[offset:len(i) - 1])  
    elif "L1D_total_cache_accesses" in i:
      collectedMetrics["l1dAcc"] = int(i[len("\tL1D_total_cache_accesses = "):len(i) - 1])
    elif "L1D_total_cache_misses" in i:
      collectedMetrics["l1dMiss"] = int(i[len("\tL1D_total_cache_misses = "):len(i) - 1])
    elif "L1D_total_cache_reservation_fails" in i:
      collectedMetrics["l1dResFail"] = int(i[len("\tL1D_total_cache_reservation_fails = "):len(i) - 1])
    elif "gpgpu_n_stall_shd_mem = " in i:
      collectedMetrics["stallShdMem"] = int(i[len("gpgpu_n_stall_shd_mem = "):len(i) - 1])
    elif "gpgpu_n_mem_read_global = " in i:
      collectedMetrics["nofMemReadGlobal"] = int(i[len("gpgpu_n_mem_read_global = "):len(i) - 1])
    elif "gpgpu_n_mem_write_global = " in i:
      collectedMetrics["nofMemWriteGlobal"] = int(i[len("gpgpu_n_mem_write_global = "):len(i) - 1])
    elif "gpgpu_n_load_insn  =" in i:
      collectedMetrics["nofLoadInst"] = int(i[len("gpgpu_n_load_insn  = "):len(i) - 1])
    elif "gpgpu_n_store_insn = " in i:
      collectedMetrics["nofStoreInst"] = int(i[len("gpgpu_n_store_insn = "):len(i) - 1])
    elif "gpgpu_n_shmem_insn = " in i:
      collectedMetrics["nofShmMemInst"] = int(i[len("gpgpu_n_shmem_insn = "):len(i) - 1])
    elif "Stall:" in i:
      stall = 0
      w0_idle = 0
      w0_scoreboard = 0
      offset = 0
      for j in range (0, 35):
        if j == 0:
          offset += len("Stall:")
        elif j == 1:
          offset += len("W0_Idle:")
        elif j == 2:
          offset += len("W0_Scoreboard:")
        elif j in [3,4,5,6,7,8,9,10,11]:
          offset += len("W1:")
        elif j == 34:
          collectedMetrics["w" + str(j-2)] = int(i[offset+4:len(i)-1])
          break
        else:
          offset += len("W10:")
        counter = 0
        value = ''
        while True:
          if i[offset + counter] != '\t':
            value += i[offset + counter]
            counter += 1
          else:
            break
        offset += len('\t') + len(value)

        if j == 0:
          collectedMetrics["stall"] = int(value)
        elif j == 1:
          collectedMetrics["w0Idle"] = int(value)
        elif j == 2:
          collectedMetrics["w0ScoreBoard"] = int(value)
        else:
          collectedMetrics["w" + str(j-2)] = int(value)

    elif "maxmflatency = " in i:
      collectedMetrics["maxMFLatency"] = int(i[len("maxmflatency = "):len(i) - 1])
    elif "averagemflatency = " in i:
      collectedMetrics["avMFLatency"] = int(i[len("averagemflatency = "):len(i) - 1])
    elif "Row_Buffer_Locality = " in i:
      collectedMetrics["rowBufLoc"] += float(i[len("Row_Buffer_Locality = "):len(i) - 1])
    elif "L2_total_cache_accesses =" in i:
      collectedMetrics["l2Acc"] = int(i[len("L2_total_cache_accesses = "):len(i) - 1])
    elif "L2_total_cache_misses = " in i:
      collectedMetrics["l2Miss"] = int(i[len("L2_total_cache_misses = "):len(i) - 1])
    elif "L2_total_cache_reservation_fails = " in i:
      collectedMetrics["l2ResFail"] = int(i[len("L2_total_cache_reservation_fails = "):len(i) - 1])

  collectedMetrics["rowBufLoc"] = float(collectedMetrics["rowBufLoc"]) / 12
  return collectedMetrics

def seperateSequentialKernelMetrics(m):
  keys = list(m.keys())
  k = m["nofKernels"]

  for i in reversed(range(1, k)):
    m["kernel_" + str(i)]["l1dAcc"] = m["kernel_" + str(i)]["l1dAcc"] - m["kernel_" + str(i-1)]["l1dAcc"]  
    m["kernel_" + str(i)]["l1dMiss"] = m["kernel_" + str(i)]["l1dMiss"] - m["kernel_" + str(i-1)]["l1dMiss"]
    m["kernel_" + str(i)]["l1dResFail"] = m["kernel_" + str(i)]["l1dResFail"] - m["kernel_" + str(i-1)]["l1dResFail"]
    m["kernel_" + str(i)]["l2Acc"] = m["kernel_" + str(i)]["l2Acc"] - m["kernel_" + str(i-1)]["l2Acc"]
    m["kernel_" + str(i)]["l2Miss"] = m["kernel_" + str(i)]["l2Miss"] - m["kernel_" + str(i-1)]["l2Miss"]
    m["kernel_" + str(i)]["l2ResFail"] = m["kernel_" + str(i)]["l2ResFail"] - m["kernel_" + str(i-1)]["l2ResFail"]
    m["kernel_" + str(i)]["stallShdMem"] = m["kernel_" + str(i)]["stallShdMem"] - m["kernel_" + str(i-1)]["stallShdMem"]  
    m["kernel_" + str(i)]["nofMemReadGlobal"] = m["kernel_" + str(i)]["nofMemReadGlobal"] - m["kernel_" + str(i-1)]["nofMemReadGlobal"]  
    m["kernel_" + str(i)]["nofMemWriteGlobal"] = m["kernel_" + str(i)]["nofMemWriteGlobal"] - m["kernel_" + str(i-1)]["nofMemWriteGlobal"]  
    m["kernel_" + str(i)]["nofLoadInst"] = m["kernel_" + str(i)]["nofLoadInst"] - m["kernel_" + str(i-1)]["nofLoadInst"]  
    m["kernel_" + str(i)]["nofStoreInst"] = m["kernel_" + str(i)]["nofStoreInst"] - m["kernel_" + str(i-1)]["nofStoreInst"]  
    m["kernel_" + str(i)]["nofShmMemInst"] = m["kernel_" + str(i)]["nofShmMemInst"] - m["kernel_" + str(i-1)]["nofShmMemInst"]  

  for i in range(1, k-1):
    m["kernel_" + str(i)]["l1dMiss"] = float(m["kernel_" + str(i)]["l1dMiss"]) / float(m["kernel_" + str(i)]["l1dAcc"])
    m["kernel_" + str(i)]["l1dResFail"] = float(m["kernel_" + str(i)]["l1dResFail"]) / float(m["kernel_" + str(i)]["l1dAcc"])
    m["kernel_" + str(i)]["l2Miss"] = float(m["kernel_" + str(i)]["l2Miss"]) / float(m["kernel_" + str(i)]["l2Acc"])
    m["kernel_" + str(i)]["l2ResFail"] = float(m["kernel_" + str(i)]["l2ResFail"]) / float(m["kernel_" + str(i)]["l2Acc"])

  m["kernel_" + str(0)]["l1dMiss"] = float(m["kernel_" + str(0)]["l1dMiss"]) / float(m["kernel_" + str(0)]["l1dAcc"])
  m["kernel_" + str(0)]["l1dResFail"] = float(m["kernel_" + str(0)]["l1dResFail"]) / float(m["kernel_" + str(0)]["l1dAcc"])
  m["kernel_" + str(0)]["l2Miss"] = float(m["kernel_" + str(0)]["l2Miss"]) / float(m["kernel_" + str(0)]["l2Acc"])
  m["kernel_" + str(0)]["l2ResFail"] = float(m["kernel_" + str(0)]["l2ResFail"]) / float(m["kernel_" + str(0)]["l2Acc"])

  return m

def collectPerAppMetrics(app):
  resultantFiles = list(app.keys())
  for i in resultantFiles:
    file = open(i, 'r')
    nofLines = len(file.readlines())
    file.close()

    nofKernel = 0
#    print(i, app[i]["nofKernels"])
    file = open(i, 'r')
    for j in range(0, nofLines):
      line = file.readline()
      kernel_metrics = []
      if "GPGPU-Sim PTX: pushing kernel" in line:
        gridDim = ''
        blockDim = ''
        ctr = 0
        while True:
          if line[ctr] == '=':
            ctr += 3
            break
          ctr += 1

        while True:
          if line[ctr] == ')':
            ctr += 1
            break
          gridDim += line[ctr]
          ctr += 1
        ctr += len(" blockDim = (")

        while True:
          if line[ctr] == ')':
            ctr += 1
            break
          blockDim += line[ctr]
          ctr += 1

        while True:
          line = file.readline()
          if "gpgpu_simulation_time" in line or j >= nofLines:
            break
          else:
            kernel_metrics.append(line)
            j += 1

        if j >= nofLines:
          break

        app[i]["kernel_" + str(nofKernel)] = collectPerKernelMetrics(kernel_metrics)
        app[i]["kernel_" + str(nofKernel)]["gridDim"] = gridDim
        app[i]["kernel_" + str(nofKernel)]["blockDim"] = blockDim
#        print("kernel_" + str(nofKernel))
        nofKernel += 1
    app[i] = seperateSequentialKernelMetrics(app[i])
#    if "frb" in i:
#      print(app)
    
  return app


apps = {}
for i in cuda_applications:
  apps[i] = countExperimentsAndNames(i)

for i in cuda_applications:
  openAppTest(apps[i])

for i in cuda_applications:
  apps[i] = countNofKernels(apps[i])
  apps[i] = collectPerAppMetrics(apps[i])
  exportExcel(apps[i])

#print(list(apps.keys()))
#y = np.array([35, 25, 25, 15])
#mylabels = ["Apples", "Bananas", "Cherries", "Dates"]
#plt.pie(y, labels = mylabels)
#plt.legend(title = "Four Fruits:")
#plt.show() 

