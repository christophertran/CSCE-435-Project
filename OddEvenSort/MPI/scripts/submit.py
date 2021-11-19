import os

def submitJobs():
    jobIDs = []
    jobIDLineIndex = 0
    jobIDIndex = 3

    minProcesses = 8
    maxProcesses = 256
    currProcesses = minProcesses

    minInputSize = 2560
    maxInputSize = 2560000
    currInputSize = minInputSize

    # inputTypes = ['random', 'sorted', 'reversed']
    inputTypes = ['random']

    os.system('module load intel/2020b && mpiicc OddEvenSort.cpp -o OddEvenSort.exe')
    file = open('/scratch/user/tchristopher457/Working/csce435project/OddEvenSort/MPI/mpi.grace_job', 'r')
    lines = file.readlines()
    file.close()

    for inputType in inputTypes:
        while currInputSize <= maxInputSize:
            while currProcesses <= maxProcesses:
                file = open('/scratch/user/tchristopher457/Working/csce435project/OddEvenSort/MPI/mpi.grace_job', 'w')
                lines[-1] = 'mpirun -np ' + str(currProcesses) + ' ./OddEvenSort.exe ./input/' + str(currInputSize) + '_' + inputType + '_numbers.bin'
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
    jobIDs = submitJobs()
    print(jobIDs)

__main__()