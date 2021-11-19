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
##First Executable Line
# 
module load intel/2020b       # load Intel software stack
sbatch mpirun -np $1 test $2