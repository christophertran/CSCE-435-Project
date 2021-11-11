# Odd-Even Transposition Sort

## MPI Implementation

### Step 1: Compile and generate random numbers

#### Step 1a: Compile RandomNumberGenerator.cpp

```txt
g++ RandomNumberGenerator.cpp -o RandomNumberGenerator
```

#### Step 1b: Generate binary file with random numbers

```txt
./RandomNumberGenerator 100
```

### Step 2: Compile and sort random numbers

#### Step 2a: Load intel module

```txt
module load intel/2020b
```

#### Step 2b: Compile OddEvenSort.cpp

```txt
mpiicc OddEvenSort.cpp -o OddEvenSort
```

#### Step 2c: Modify number of processes and random numbers binary in *mpi.grace_job* file

```txt
mpirun -np 4 ./OddEvenSort 100numbers.bin
```

#### Step 2d: Submit batch job to grace cluster

```txt
sbatch mpi.grace_job
```
