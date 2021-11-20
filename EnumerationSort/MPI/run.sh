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
mpiicc EnumerationSort.cpp -o EnumerationSort.exe  
rm output.*
rm *.bin
rm core.*

# compile random number generator
g++ RandomNumberGenerator.cpp -o RandomNumberGenerator

# generate binary files for random numbers
for size in 200 400 600 800 1000
do
    for type in 0 1 2
    do
        ./RandomNumberGenerator $size $type
    done
done

./run.grace_job 200 "200_random_numbers.bin"
./run.grace_job 200 "200_reversed_numbers.bin"
./run.grace_job 200 "200_sorted_numbers.bin"
./run.grace_job 400 "400_random_numbers.bin"
./run.grace_job 400 "400_reversed_numbers.bin"
./run.grace_job 400 "400_sorted_numbers.bin"
./run.grace_job 600 "600_random_numbers.bin"
./run.grace_job 600 "600_reversed_numbers.bin"
./run.grace_job 600 "600_sorted_numbers.bin"
./run.grace_job 800 "800_random_numbers.bin"
./run.grace_job 800 "800_reversed_numbers.bin"
./run.grace_job 800 "800_sorted_numbers.bin"
./run.grace_job 1000 "1000_random_numbers.bin"
./run.grace_job 1000 "1000_reversed_numbers.bin"
./run.grace_job 1000 "1000_sorted_numbers.bin"





