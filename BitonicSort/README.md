# How to build a project
## Release:
```
  $ mkdir build
  $ cd build
  $ cmake -DKERNEL=$SRC/kernels/kernel.cl ..
  $ make
```
* Note: Default path to the kernel is "../kernels/kernel.cl"

* Note: You also have an opportunity to compare its performance with std::sort. To do it run *time_profile* with your test in input
## Debug:
```
  $ mkdir build
  $ cd build
  $ cmake -DDebug=on ..
  $ make
```
### Run *tests* to check the correctness
---
## Notes:
* if you have Nvidia video card try:
```
  $ cmake -DNVIDIA=on .. 
```
* Make sure you have OpenCL installed!
---
## Compiler settings:
```
  $ cxx_std_17
```
---
### In the folder 'build' executiable files 'bitonic_sort', 'time_profile' and  test-file 'tests' (with -DDebug) will appear
---
## Input data: 
* **N** --- size of a sequence
* **N** --- Data (integer or floating)  
## Output data: 
* Sorted sequence and sorting time
### ***example:*** 
* **INPUT:** ---> 4 5 6 7 1 

* **OUTPUT:** ---> 

``` 
1 5 6 7 
CPU time: **N**ms
GPU time: **M**ms 
```

---
# Useful materials: 
* https://www.geeksforgeeks.org/bitonic-sort/ - the description of algorithm
---
