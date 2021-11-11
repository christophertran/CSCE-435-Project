/**
 * @file OddEvenSort.cpp
 * @author Eren Albayrak & Marius Wichtner (Modified and Updated by Christopher Tran)
 * @brief Initial source code taken from the github repository below.
 *        https://github.com/erenalbayrak/Odd-Even-Sort-mit-MPI
 *
 *        The code has been modified and updated for CSCE-435 by Christopher Tran
 *        Modifications include adding timing and evaluation metrics for determining 
 *        the sorting algorithm's performance. Additionally, comments were added for
 *        clarification, learning, and understanding purposes.
 */

#include <mpi.h>

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>

/**
 * @brief Utility function that reads numbers from generated binary file and stores
 *        it within an array
 * 
 * @param data
 * @param binary_file 
 * @param rank 
 * @param count_processes 
 * @param data_size 
 * @return int 
 */
int fill_array_from_binary_file(int **data,
                                char *binary_file,
                                long rank,
                                int count_processes,
                                unsigned long &data_size) {
    std::ifstream bin_file(binary_file, std::ios::in | std::ios::binary);
    bin_file.seekg(0, std::ios::end);

    const long int count_all_bytes = bin_file.tellg();
    data_size = (count_all_bytes / sizeof(int) / count_processes);

    if (data_size < 1) {
        std::cout << "The amount of processes is higher than the amount of values. In this case not every process will become values." << std::endl;
        return EXIT_FAILURE;
    } else if ((count_all_bytes / sizeof(int)) % count_processes != 0) {
        std::cout << "The amount of values have to be even on every process. Otherwise the sorting would be incorrect." << std::endl;
        return EXIT_FAILURE;
    }

    *data = new int[data_size];

    bin_file.seekg(rank * sizeof(int) * data_size, std::ios::beg);
    bin_file.read(reinterpret_cast<char *>(*data), sizeof(int) * data_size);

    return EXIT_SUCCESS;
}

/**
 * @brief 
 * 
 * @param phase 
 * @param rank 
 * @return int 
 */
int findPartner(int phase, int rank) {
    int partner;

    /* if it's an even phase */
    if (phase % 2 == 0) {
        /* if we are an even process */
        if (rank % 2 == 0) {
            partner = rank + 1;
        } else {
            partner = rank - 1;
        }
    } else {
        /* it's an odd phase - do the opposite */
        if (rank % 2 == 0) {
            partner = rank - 1;
        } else {
            partner = rank + 1;
        }
    }
    return partner;
}

/**
 * @brief 
 * 
 * @param a 
 * @param b 
 * @return int 
 */
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

/**
 * @brief 
 * 
 * @param data 
 * @param rank 
 * @param count_processes 
 * @param data_size 
 */
void parallel_sort(int *data, int rank, int count_processes, unsigned long data_size) {
    const unsigned long concat_data_size = data_size * 2;

    auto *other = new int[data_size];
    auto *concatData = new int[concat_data_size];

    for (int i = 0; i < count_processes; i++) {
        int partner = findPartner(i, rank);
        if (partner < 0 || partner >= count_processes)
            continue;

        if (rank % 2 == 0) {
            MPI_Send(data, (int)data_size, MPI_INT, partner, 0, MPI_COMM_WORLD);
            MPI_Recv(other, (int)data_size, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else {
            MPI_Recv(other, (int)data_size, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(data, (int)data_size, MPI_INT, partner, 0, MPI_COMM_WORLD);
        }

        std::merge(data, data + data_size,
                   other, other + data_size,
                   concatData);
        std::qsort(concatData, concat_data_size, sizeof(int), compare);

        auto posHalfConcatData = concatData + data_size;
        if (rank < partner)
            std::copy(concatData, posHalfConcatData, data);
        else
            std::copy(posHalfConcatData, concatData + concat_data_size, data);
    }
}

/**
 * Compile:      mpic++ OddEvenSort.cpp -o OddEvenSort
 * Example-Call: mpirun -np 4 ./OddEvenSort "<numbers_file.bin>"
 * */
int main(int argCount, char **argValues) {
    int rank, count_processes;

    MPI_Init(&argCount, &argValues);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count_processes);

    int *data;
    unsigned long data_size;
    int status = fill_array_from_binary_file(&data, argValues[1], rank, count_processes, data_size);

    if (status == EXIT_FAILURE) {
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    parallel_sort(data, rank, count_processes, data_size);

    MPI_Finalize();
    return EXIT_SUCCESS;
}
