# How to build a project
```
  $ mkdir build
  $ cd build
  $ cmake -DGPU=on ..
  $ make
```
* Note: flag  **-DGPU** run sorting on GPU, but you also have an opportunity to run simple bitonic sort on CPU or even std::sort. To do it run *cmake* with relevants parameters below:
```
cmake -DCPU=on -DGPU=off -DSTD=off ..   
```
OR
```
cmake -DCPU=off -DGPU=on -DSTD=off ..   
```
## Make sure you have OpenCL installed!
---
## If you have some troubles with building the project, try to install GoogleTests:
#Ubuntu:
```
  $ sudo apt-get install libgtest-dev
  $ cd /usr/src/gtest
  $ sudo cmake CMakeLists.txt
  $ sudo make
```
#Arch:
```
  $ sudo pacman -S gtest
```
---
## Compiler settings:
```
  $ g++ -Wall -Wextra -Wpedantic -O2
```
---
---
### In the folder 'build' executiable files 'bitonic_sort' and  test-file 'tests' will appear
---
## Input data: 
* **N** --- size of a sequence
* **N** --- Data (integer or floating)  
## Output data: 
* Sorted sequence and sorting time
### ***example:*** 
* **INPUT:** ---> 4 5 6 7 1 

* **OUTPUT:** ---> 1 5 6 7 elapsed time: **N**s 

---
# Useful materials: 
* https://www.geeksforgeeks.org/bitonic-sort/ - the description of algorithm
---