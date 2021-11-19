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
    random_jobIDs = [1952375, 1952376, 1952377, 1952378, 1952379, 1952380, 1952381, 1952382, 1952383, 1952384, 1952385, 1952386, 1952388, 1952389, 1952390, 1952392, 1952393, 1952394, 1952395, 1952396, 1952397, 1952398, 1952399, 1952400]
    sorted_jobIDs = [1952405, 1952407, 1952408, 1952409, 1952411, 1952412, 1952413, 1952414, 1952415, 1952416, 1952417, 1952418, 1952419, 1952420, 1952421, 1952422, 1952423, 1952424, 1952425, 1952426, 1952427, 1952428, 1952429, 1952430]
    reversed_jobIDs = [1952441, 1952442, 1952447, 1952448, 1952449, 1952450, 1952451, 1952452, 1952453, 1952454, 1952455, 1952456, 1952457, 1952458, 1952459, 1952460, 1952461, 1952462, 1952463, 1952464, 1952465, 1952466, 1952467, 1952468]
    data = parseOutput(reversed_jobIDs)
    print(data)

__main__()