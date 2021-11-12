# Parallel Merge Sort

## MPI Implementation

---

### **Step 1: Compile and generate random numbers**


####  - Step 1a: Load intel module

```txt
module load intel/2020b
```

####  - Step 1b: Compile RandomNumberGenerator.cpp

```txt
g++ RandomNumberGenerator.cpp -o RandomNumberGenerator.exe
```

####  - Step 1c: Generate binary file with random numbers

```txt
./RandomNumberGenerator 100
```
---

### **Step 2: Compile and sort random numbers**

####  - Step 2a: Load intel module

```txt
module load intel/2020b
```

####  - Step 2b: Compile ParallelMergeSort.cpp

```txt
mpiicc ParallelMergeSort.cpp -o ParallelMergeSort.exe
```

####  - Step 2c: Modify number of processes and random numbers binary in *mpi.grace_job* file

```txt
mpirun -np 4 ./ParallelMergeSort.exe 100numbers.bin
```

####  - Step 2d: Submit batch job to grace cluster

```txt
sbatch mpi.grace_job
```
