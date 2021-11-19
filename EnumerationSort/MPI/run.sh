#!/bin/bash
##ENVIRONMENT SETTINGS; CHANGE WITH CAUTION
#SBATCH --export=NONE            #Do not propagate environment
#SBATCH --get-user-env=L         #Replicate login environment
#
##NECESSARY JOB SPECIFICATIONS
#SBATCH --job-name=JobName       #Set the job name to "JobName"
#SBATCH --time=0:30:00           #Set the wall clock limit to 0hr and 30min
#SBATCH --nodes=16               #Request 16 node
#SBATCH --ntasks-per-node=4      #Request 4 tasks/cores per node
#SBATCH --mem=8G                 #Request 8GB per node 
#SBATCH --output=output."$1_$2"  #Send stdout/err to "output.[jobID]" 
#
module load intel/2020b       # load Intel software stack
mpiicc EnumerationSort.cpp -o EnumerationSort.exe #  
rm output.*
rm *.bin
rm core.*

# compile random number generator
g++ RandomNumberGenerator.cpp -o RandomNumberGenerator

# generate binary files for random numbers
for size in 2560 25600 256000 2560000
do
    for type in 0 1 2
    do
        ./RandomNumberGenerator $size $type
    done
done

./run.grace_job  2560 "2560_random_numbers.bin"
./run.grace_job  2560 "2560_reversed_numbers.bin"
./run.grace_job  2560 "2560_sorted_numbers.bin"
./run.grace_job  25600 "25600_random_numbers.bin"
./run.grace_job  25600 "25600_reversed_numbers.bin"
./run.grace_job  25600 "25600_sorted_numbers.bin"
./run.grace_job  256000 "256000_random_numbers.bin"
./run.grace_job  256000 "256000_reversed_numbers.bin"
./run.grace_job  256000 "256000_sorted_numbers.bin"
./run.grace_job  2560000 "2560000_random_numbers.bin"
./run.grace_job  2560000 "2560000_reversed_numbers.bin"
./run.grace_job  2560000 "2560000_sorted_numbers.bin"





