import os

def submitJobs(inputType):
    jobIDs = []
    jobIDLineIndex = 0
    jobIDIndex = 3

    minProcesses = 8
    maxProcesses = 256
    currProcesses = minProcesses

    minInputSize = 2560
    maxInputSize = 2560000
    currInputSize = minInputSize

    #inputTypes = ['random', 'sorted', 'reversed']
    #inputTypes = ['random']

    os.system('module load intel/2020b && mpiicc ParallelMergeSort.cpp -o ParallelMergeSort.exe -no-multibyte-chars')
    file = open('/scratch/user/kelton.chesshire/Group_Project/csce435project/ParallelMergeSort/MPI/mpi.grace_job', 'r')
    lines = file.readlines()
    file.close()

    #for inputType in inputTypes:
    while currInputSize <= maxInputSize:
        while currProcesses <= maxProcesses:
            file = open('/scratch/user/kelton.chesshire/Group_Project/csce435project/ParallelMergeSort/MPI/mpi.grace_job', 'w')
            lines[-1] = 'mpirun -np ' + str(currProcesses) + ' ./ParallelMergeSort.exe ./input/' + str(currInputSize) + '_' + inputType + '_numbers.bin'
            print(lines[-1])
            
            file.writelines(lines)
            file.close()

            stream = os.popen('sbatch mpi.grace_job')
            jobIDs.append(int(stream.readlines()[jobIDLineIndex].split()[jobIDIndex]))

            currProcesses *= 2

        currProcesses = minProcesses
        currInputSize *= 10

    currInputSize = minInputSize
    
    return jobIDs

def __main__():
    random_jobIDs = submitJobs('random')
    sorted_jobIDs = submitJobs('sorted')
    reversed_jobIDs = submitJobs('reversed')
    print("-----------\n")
    print("Random:\n")
    print(random_jobIDs)
    print("\n-----------\n")
    
    print("-----------\n")
    print("Sorted:\n")
    print(sorted_jobIDs)
    print("\n-----------\n")
    
    print("-----------\n")
    print("Reversed:\n")
    print(reversed_jobIDs)
    print("\n-----------\n")

__main__()