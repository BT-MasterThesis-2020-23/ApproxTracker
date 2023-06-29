## For the installation and build
---------------------------------
Detailed documentation on what GPGPU-Sim models are, how to configure them, and a
guide to the source code can be found here: <http://gpgpu-sim.org/manual/>.
Instructions for building Doxygen source code documentation are included below.
Detailed documentation on GPUWattch, including how to configure it and a guide
to the source code, can be found here: <http://gpgpu-sim.org/gpuwattch/>.

**GPGPU-Sim dependencies:** ```gcc, g++, make, makedepend, xutils, bison, flex, zlib, CUDA Toolkit```

**GPGPU-Sim documentation dependencies:** ```doxygen, graphvi```

**AerialVision dependencies:** ```python-pmw, python-ply, python-numpy, libpng12-dev, python-matplotlib```

**CUDA SDK dependencies:** ```install libxi-dev libxmu-dev libglut3-dev```

$source setup_environment <build_type> 
 - For debugging, build_type -> ```debug```
 - For normal release -> you dont need to specify build_type, blank as empty.

To build the simulator ```user$ make```, clean executables of the simulator ```user$ make clean```, build the Doxygen generated documentation ```user$ make docs```, clean the docs ```user$ make cleandocs```. This doxygen-generated documentation will help you to understand classes, templates, functions, etc.

## ApproxTrackerL1D and ApproxTrackerL2

![Screenshot from 2023-06-29 22-00-37](https://github.com/topcuburak/ApproxTracker/assets/73446582/1ee873e9-9c21-4d36-9741-23f3a61bade9)

1- **ApproxTrackerL1D** flushes memory requests that will probably cause a miss on the L1 data cache for load operations within the same SM. It also writes zero to the operands of the corresponding memory request. <br>
2- **ApproxTrackerL2** flushes memory requests that will probably cause a miss on one of the L2 caches for load operations. It also writes zero to the operands of the corresponding memory request.

| Config Type | Descriptions |
|:------|:-------------|
| -l1dApproximator 1 | This flag enables the ApproxTrackerL1D.|
| -l1dSlicingAccessInterval 2000 | This configuration option helps determine slicing interval size to capture temporal memory problems through the runtime.|
| -l2Approximator 0 | enables ApproxTrackerL2. | 
| -l2SlicingAccessInterval 2000 | Slicing interval size for ApproxTrackerL2. |
| -errorPercentage 100 | You can set the error percentage with this flag. For example, you can think of 100 as 1%, 50 as 2%, and 200 as 0.5%. |
| -kernelSpecificApprox 1 | You may simulate the approximator for a kernel within multiple of them. Here, you need to specify the kernel id, which starts from 0. |
| -kernelIdApprox 0 | kernel id |

**Note:** GPPRMon metric collection parts are still valid for this simulator version. 

## Naive and simulation results
Inside the ApproxTrackerExperiments folder, we share the simulation results for Naive, ApproxTrackerL1D, and ApproxTrackerL2 with both 2% (50) and 1% (100) flushes of missed memory requests. 

![Screenshot from 2023-06-29 22-00-54](https://github.com/topcuburak/ApproxTracker/assets/73446582/2fd04861-9710-4dee-ab00-f94797113e06)
