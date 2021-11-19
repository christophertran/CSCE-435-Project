def parseOutput(jobIDs):
    data = {}

    for jobID in jobIDs:
        pathBase = '/scratch/user/kelton.chesshire/Group_Project/csce435project/ParallelMergeSort/MPI/output.'
        file = open(pathBase + str(jobID))

        stream = file.readlines()

        for line in stream:
            if line != '==========\n':
                splitLine = line.split(':')
                key = splitLine[0]
                value = splitLine[-1][:-1].strip()
                data[key] = data.get(key, []) + [value]

    return data

def __main__():
    random_jobIDs = [1954892, 1954893, 1954895, 1954896, 1954897, 1954898, 1954899, 1954900, 1954901, 1954902, 1954903, 1954904, 1954905, 1954906, 1954907, 1954908, 1954911, 1954913, 1954915, 1954916, 1954917, 1954918, 1954919, 1954920]
    sorted_jobIDs = [1954921, 1954922, 1954923, 1954924, 1954925, 1954926, 1954927, 1954928, 1954931, 1954932, 1954933, 1954934, 1954935, 1954936, 1954937, 1954938, 1954939, 1954940, 1954941, 1954942, 1954943, 1954944, 1954945, 1954946]
    reversed_jobIDs = [1954947, 1954948, 1954949, 1954950, 1954951, 1954952, 1954954, 1954955, 1954956, 1954957, 1954958, 1954959, 1954960, 1954961, 1954962, 1954963, 1954964, 1954965, 1954966, 1954967, 1954969, 1954970, 1954971, 1954972]
    reversed_data = parseOutput(reversed_jobIDs)
    random_data = parseOutput(random_jobIDs)
    sorted_data = parseOutput(sorted_jobIDs)
    
    print("Random:\n")
    print(random_data)
    print("\n \n")
    
    print("Sorted:\n")
    print(sorted_data)
    print("\n \n")

    print("Reversed:\n")
    print(reversed_data)
    print("\n \n")


__main__()