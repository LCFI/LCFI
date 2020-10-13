LCFI
---

Lossy compression has been widely used in HPC applications to significantly reduce the data movement and memory overheads. However, the impact of compression errors in HPC programs is not clear yet to many HPC programs, and current lossy compression methods have not been optimized for minimizing this impact. 

LCFI is a **L**ossy **C**ompression **F**ault **I**njection tool based on LLVM that injects faults into the LLVM IR of the application source code. Users can use LCFI to inject lossy compression errors to any variable in an HPC program with different error distributions and error bounds. In this way, you can use LCFI to select the optimal lossy compressor and compression configuration for any HPC program that needs to use lossy compression. Moreover, lossy compression developers can use LCFI to customize the error distribution and compression scheme, which can help them design a better lossy compressor with higher data fidelity. 

LCFI is modified based on [LLFI](https://github.com/DependableSystemsLab/LLFI) [1] developed by the University of British Columbia. 

[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

(C) 2020 by Washington State University and University of Iowa. See [COPYRIGHT](https://github.com/LCFI/LCFI/blob/master/LICENSE.txt) in top-level directory.

* Developers: Baodi Shan (lwshanbd@gmail.com)
* Supervisors: Dingwen Tao (dingwen.tao@wsu.edu), Guanpeng Li (guanpeng-li@uiowa.edu)
* Contributors: Aabid Shamji (aabid-shamji@uiowa.edu)

**If you have any questions about the code, please feel free to create an issue or directly contact us.**


## Install

Different from LLFI, you have to install LCFI dependencies manually.

Dependencies:
  
  1. 64 Machine with 64 bit Linux
  2. CMake (minimum v2.8)
  3. Python 3 and above
  4. Python YAML library (PyYAML)
  5. Clang v3.4
  6. LLVM v3.4, built with CMake
    * Build llvm-3.4 **WITH CMAKE** using flag `-DLLVM_REQUIRES_RTTI=1`. [Instructions](http://llvm.org/docs/CMake.html)
    * Remember to run `make` in the llvm build directory after running `cmake`.


Building:
High Performance Computing, Compression for Big Data System, data management on modern hardware

1. Edit the `compile.sh`, set the `<path where you want to build LCFI>`, `<LLVM source root dir>`, `<LLVM CMake build root dir>`
2. Run the compile.sh


## Demo

1. Copy the ./demo/ directory to your project directory.

2. Change to your demo directory Build a single IR file with the LCFI tool GenerateMakefile

```
<LCFI_BUILD_ROOT>/tools/GenerateMakefile --readable --all

make
```


3. Instrument demo with calls to LLFI libraries and create executables under *llfi* directory
```
<LCFI_BUILD_ROOT>/bin/instrument --readable demo.ll
```
4. Run `demo` executable with profiling functions instrumented
```
<LCFI_BUILD_ROOT>/bin/profile ./llfi/demo-profiling.exe
```
5. Run `demo` executable with fault injection functions instrumented
```
<LCFI_BUILD_ROOT>/bin/injectfault ./llfi/demo-faultinjection.exe
```


After fault injection, output from LCFI and the tested application can be found
in the *llfi* directory.

|     Directory         |                 Contents                       |
| ----------------------| ---------------------------------------------- |
| *std_output*          | Piped STDOUT from the tested application       |
| *llfi_stat_output*    | Fault injection statistics                     |
| *error_output*        | Failure reports (program crashes, hangs, etc.) |
| *trace_report_output* | Faults propogation report files and graph      |

## Running

Copy files in <LCFI_BUILD_ROOT>/demo-program/set/ to your project directory.

Change to your project directory. Build a single IR file with the LCFI tool GenerateMakefile

```
<LCFI_BUILD_ROOT>/tools/GenerateMakefile --readable --all
```

```
make
```


Instrument <your_program> with calls to LLFI libraries and create executables under *llfi* directory
```
<LCFI_BUILD_ROOT>/bin/instrument --readable <your_program>.ll
```


Then, set the `set.yaml` to set the variable you want to inject. If your variable is in a for-loop, you can also set which loop number you want to inject. 
> Tips: Only parametric variable is **non-init**, global and local variables' *variable_init* are all **true**.

```yaml
variable_num: 1
loop_num: <loop_num> # optional
fi_type: <fi_type>


option:
  - function_name: <function_name>
    variable_name: <variable_name>
    variable_init: <true/false>
    variable_location: <variable_location:>
    in_arr: <true/false>
    in_loop: <true/false>

```

Next, run the setinput.py, you will get set input.yaml.

```
python3 setinput.py
```

Run executable program with profiling functions instrumented
```
<LCFI_BUILD_ROOT>/bin/profile ./llfi/<your_program>-profiling.exe
```
Run executable program with fault injection functions instrumented
```
<LCFI_BUILD_ROOT>/bin/injectfault ./llfi/<your_program>-faultinjection.exe
```

After fault injection, output from LCFI and the tested application can be found
in the *llfi* directory.

|     Directory         |                 Contents                       |
| ----------------------| ---------------------------------------------- |
| *std_output*          | Piped STDOUT from the tested application       |
| *llfi_stat_output*    | Fault injection statistics                     |
| *error_output*        | Failure reports (program crashes, hangs, etc.) |
| *trace_report_output* | Faults propogation report files and graph      |


## Custom Fault

In LCFI, you can customize your own falut using the FIDL tool or you can also set a specific value with faults which we have provided you with.


### Set your value used our injector

Open <LCFI_SRC_ROOT>/tools/FIDL/FIDL.yaml, select the injector you want to use and the value.

```
Failure_Class: LCFI
Failure_Mode: <Set Your Own Mode Name>

New_Failure_Mode:
    Trigger: 
      call:
        [all]
    Target:
      src: 
        all: [0]
    Action:
        Perturb: <Injector_Name>
        value: <Your_Value>

```

We now have 8 injectors corresponding to 8 different situations:

|          InjectorName           | Type of Data |
| --------------------------------| ------------ |
| *UniformDistribution*           | double       |
| *UniformDistributionByRate*     | double       |
| *NormalDistribution*            | double       |
| *NormalDistributionByRate*      | double       |
| *floatUniformDistribution*      | float        |
| *floatUniformDistributionByRate*| float        |
| *floatNormalDistribution*       | float        |
| *floatNormalDistributionByRate* | float        |


Run the `FIDL-algorithm.py` 

```sh
python3 FIDL-algorithm.py -a FIDL.yaml
```

Finally, go back to <LCFI_SRC_ROOT> and recompile it.

```
./compile.sh
```



### Add a new fault model

 Open the <LCFI_SRC_ROOT>/runtime_lib/_SoftwareFaultInjectors.cpp, as you can see, a standard fault setting is like this:
   
```C++
class UniformDistribution: public SoftwareFaultInjector {
	public:

	double randdouble(double Lower, double Upper)
	{
		double temp;
		if (Lower > Upper)
		{
			temp = Upper;
			Upper = Lower;
			Lower = temp;
		}

		return rand() / (double)RAND_MAX * (Upper - Lower) + Lower;
	}

	virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf){
		double* newbuf = (double*) buf;
		double tmp=*newbuf;
		*newbuf=randdouble(tmp-add_val,tmp+add_val);
		return;
	}
	
	UniformDistribution(double val):add_val(val){};
	
	private:
	double add_val;
	
};

```
If you want to write your own fault, you need to rewrite the injectFault function. 

In this function, convert the buf val `char` to the `type of data` you want to use. In most situations, you need  `double` or `float` type. 
```
<Data Type>* newbuf = (<Data Type>*) buf;
```

Then, edit the content of the function. **The value of `newbuf` is the finally changed value.**

Next, change the `FIDL-algorithm.py` in  <LCFI_SRC_ROOT>/tools/FIDL/ . Please copy this code after line 574:

```python
elif '<Your_Injector_Name>' in perturb:
    injector = '<Your_Injector_Name>'
      try:
        code.append('static RegisterFaultInjector %s new <Your_Injector_Name>(%s));' % (
              insert, value))

```
Set the FIDL.yaml to specific the value you want to inject:
```yaml
Failure_Class: LCFI
Failure_Mode: <Set Your Own Mode Name>

New_Failure_Mode:
    Trigger: 
      call:
        [all]
    Target:
      src: 
        all: [0]
    Action:
        Perturb: <Your_Injector_Name>
        value: <Your_Value>
```


Run the `FIDL-algorithm.py` 

```sh
python3 FIDL-algorithm.py -a FIDL.yaml
```

Finally, go back to <LCFI_SRC_ROOT> and recompile it.

```
./compile.sh
```

## VirtualBox Image

TBD
<!--
If you want to quickly try out LLFI, an Ubuntu image with LLFI and its dependencies pre-installed 
is available [here](https://drive.google.com/file/d/0B5inNk8m9EfeM096ejdfX2pTTUU/view?usp=sharing) (2.60GB). This image is built with VirtualBox v4.3.26, with Ubuntu 14.04.2 LTS, LLVM v3.4, CMake v3.4 and the current master branch version of LLFI (as of Sep 16th, 2015).

user: `llfi`  
password: `root`

`<LLFI_SRC_ROOT>` is located under `~/Desktop/llfisrc/`.  
`<LLFI_BUILD_ROOT>` is located under `~/Desktop/llfi/`.  
`<LLVM_SRC_ROOT>` is located under `~/Desktop/llvmsrc/`.  
`<LLVM_DST_ROOT>` is located under `~/Desktop/llvm/`.  
`<LLVM_GXX_BIN_DIR >` is located under `~/Desktop/llvm/bin/`.  

Sample tests can be found under `~/Desktop/test/`.

To run it, open VirtualBox, select `File->Import Appliance...` and navigate to the `.ova` file.
--->

## References
[1] Jiesheng Wei, Anna Thomas, Guanpeng Li, and Karthik Pattabiraman. "Quantifying the accuracy of high-level fault injection techniques for hardware faults." In 2014 44th Annual IEEE/IFIP International Conference on Dependable Systems and Networks, pp. 375-382. IEEE, 2014.
