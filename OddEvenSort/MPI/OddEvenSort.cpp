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

// #define COUT

#include <mpi.h>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <cstring>

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
                                unsigned long &data_size)
{
    std::ifstream bin_file(binary_file, std::ios::in | std::ios::binary);
    bin_file.seekg(0, std::ios::end);

    const long int count_all_bytes = bin_file.tellg();
    data_size = (count_all_bytes / sizeof(int) / count_processes);

    if (data_size < 1)
    {
        std::cout << "The amount of processes is higher than the amount of values. In this case not every process will become values." << std::endl;
        return EXIT_FAILURE;
    }
    else if ((count_all_bytes / sizeof(int)) % count_processes != 0)
    {
        std::cout << "The amount of values have to be even on every process. Otherwise the sorting would be incorrect." << std::endl;
        return EXIT_FAILURE;
    }

    *data = new int[data_size];

    bin_file.seekg(rank * sizeof(int) * data_size, std::ios::beg);
    bin_file.read(reinterpret_cast<char *>(*data), sizeof(int) * data_size);

    return EXIT_SUCCESS;
}

/**
 * @brief Find 
 * 
 * @param phase 
 * @param rank 
 * @return int 
 */
int findPartner(int phase, int rank)
{
    int partner;

    /* if it's an even phase */
    if (phase % 2 == 0)
    {
        /* if we are an even process */
        if (rank % 2 == 0)
        {
            partner = rank + 1;
        }
        else
        {
            partner = rank - 1;
        }
    }
    else
    {
        /* it's an odd phase - do the opposite */
        if (rank % 2 == 0)
        {
            partner = rank - 1;
        }
        else
        {
            partner = rank + 1;
        }
    }
    return partner;
}

/**
 * @brief Comapare 2 ints, return -1, 0, or 1, respectively,
 *        when the first int is less than, equal, or greater than
 *        the second. Used for qsort.
 * 
 * @param a first int
 * @param b second int
 * @return int 
 */
int compare(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

/**
 * @brief Parallel algorithm for Odd Even Transposition Sort
 * 
 * @param data local list
 * @param rank process rank
 * @param count_processes # processes
 * @param data_size local list size
 */
void oddEvenSort(int *data, int rank, int count_processes, unsigned long data_size)
{
    const unsigned long concat_data_size = data_size * 2;

    auto *other = new int[data_size];
    auto *concatData = new int[concat_data_size];

    for (int i = 0; i < count_processes; i++)
    {
        int partner = findPartner(i, rank);
        if (partner < 0 || partner >= count_processes)
            continue;

        if (rank % 2 == 0)
        {
            MPI_Send(data, (int)data_size, MPI_INT, partner, 0, MPI_COMM_WORLD);
            MPI_Recv(other, (int)data_size, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        else
        {
            MPI_Recv(other, (int)data_size, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(data, (int)data_size, MPI_INT, partner, 0, MPI_COMM_WORLD);
        }

        std::merge(data, data + data_size,
                   other, other + data_size,
                   concatData);

        // Sort local lists using built in quick sort
        std::qsort(concatData, concat_data_size, sizeof(int), compare);

        auto posHalfConcatData = concatData + data_size;
        if (rank < partner)
            std::copy(concatData, posHalfConcatData, data);
        else
            std::copy(posHalfConcatData, concatData + concat_data_size, data);
    }

    delete other;
    delete concatData;
}

/**
 * @brief Print the contents of the global list
 * 
 * @param local_A local list
 * @param local_n local list size
 * @param my_rank process rank
 * @param p       # processes
 * @param comm    comm name
 */
void printGlobalList(int local_A[], int local_n, int my_rank, int p, MPI_Comm comm)
{
    int *global_A;

    if (my_rank == 0)
    {
        int n = p * local_n;
        global_A = new int[n];

        MPI_Gather(local_A, local_n, MPI_INT, global_A, local_n, MPI_INT, 0, comm);

#ifdef COUT
        std::cout << ("Global list:\n");
        for (unsigned int i = 0; i < n; i++)
        {
            std::cout << global_A[i] << " ";
        }
        std::cout << std::endl;
#endif

        delete global_A;
    }
    else
    {
        MPI_Gather(local_A, local_n, MPI_INT, global_A, local_n, MPI_INT, 0,
                   comm);
    }
}

/**
 * Compile:      mpic++ OddEvenSort.cpp -o OddEvenSort
 * Example-Call (No Output): mpirun -np 4 ./OddEvenSort "<numbers_file.bin>"
 * Example-Call (Output):  mpirun -np 4 ./OddEvenSort "<numbers_file.bin>" y
 **/
int main(int argCount, char **argValues)
{
    int rank, count_processes;

    MPI_Init(&argCount, &argValues);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count_processes);

    // [0]; Measure the total time it takes to ingest, sort, and gather sorted data.
    // [1]; Measure the total time it takes to gather sorted data.
    // [2]; Measure the individual data ingestion time of each process. (min, max, avg)
    // [3]; Measure the indvidual computation time of each process. (min, max, avg)
    double local_runtime[4];

    // TOTAL COMPUTATION STARTS HERE (Timing only matters for Rank 0 process)
    if (rank == 0)
    {
        local_runtime[0] = MPI_Wtime();
    }

    // INDIVIDUAL DATA INGESTION TIME STARTS HERE
    local_runtime[2] = MPI_Wtime();

    int *data;
    unsigned long data_size;
    int status = fill_array_from_binary_file(&data, argValues[1], rank, count_processes, data_size);

    if (status == EXIT_FAILURE)
    {
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    local_runtime[2] = MPI_Wtime() - local_runtime[2];
    // INDIVIDUAL DATA INGESTION TIME ENDS HERE

    // INDIVIDUAL COMPUTATION TIME STARTS HERE
    local_runtime[3] = MPI_Wtime();

    oddEvenSort(data, rank, count_processes, data_size);

    local_runtime[3] = MPI_Wtime() - local_runtime[3];
    // INDIVIDUAL COMPUTATION TIME ENDS HERE

    // TOTAL GATHERING TIME STARTS HERE (Timing only matters for Rank 0 process)
    if (rank == 0)
    {
        local_runtime[1] = MPI_Wtime();
    }

    printGlobalList(data, data_size, rank, count_processes, MPI_COMM_WORLD);

    if (rank == 0)
    {
        local_runtime[1] = MPI_Wtime() - local_runtime[1];
    }
    // TOTAL GATHERING TIME ENDS HERE (Timing only matters for Rank 0 process)

    delete data;

    if (rank == 0)
    {
        local_runtime[0] = MPI_Wtime() - local_runtime[0];
    }
    // TOTAL COMPUTATION ENDS HERE (Timing only matters for Rank 0 process)

    // [0]; Measure the total time it takes to ingest, sort, and gather sorted data.
    // [1]; Measure the total time it takes to gather sorted data.
    // [2]; Measure the individual data ingestion time of each process. (min, max, avg)
    // [3]; Measure the indvidual computation time of each process. (min, max, avg)
    double global_min[4]; // Only need [2], [3]
    double global_max[4]; // Only need [2], [3]
    double global_avg[4]; // Only need [2], [3]

    MPI_Reduce(&local_runtime, &global_min, 4, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_runtime, &global_max, 4, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_runtime, &global_avg, 4, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        std::cout << std::fixed;
        std::cout << "==========\n";
        std::cout << "Processes: " << count_processes << "\n";
        std::cout << "Input Size: " << data_size * count_processes << "\n";
        std::cout << "==========\n";
        std::cout << "Total Computation: " << local_runtime[0] << "\n";
        std::cout << "Total Gathering: " << local_runtime[1] << "\n";
        std::cout << "==========\n";
        std::cout << "Individual Data Ingestion Min: " << global_min[2] << "\n";
        std::cout << "Individual Data Ingestion Max: " << global_max[2] << "\n";
        std::cout << "Individual Data Ingestion Avg: " << global_avg[2] / count_processes << "\n";
        std::cout << "==========\n";
        std::cout << "Individual Computation Min: " << global_min[3] << "\n";
        std::cout << "Individual Computation Max: " << global_max[3] << "\n";
        std::cout << "Individual Computation Avg: " << global_avg[3] / count_processes << std::endl;
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}
