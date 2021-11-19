#include <math.h>
#include <mpi.h>
#include <string.h>

#include <fstream>
#include <iostream>

#include "stdio.h"

int fill_array_from_binary_file(int **data, char *binary_file, long rank, int count_processes, unsigned long &data_size) {
    std::ifstream bin_file(binary_file, std::ios::in | std::ios::binary);
    bin_file.seekg(0, std::ios::end);

    const long int count_all_bytes = bin_file.tellg();
    data_size = (count_all_bytes / sizeof(int) / count_processes);

    *data = new int[data_size];

    bin_file.seekg(rank * sizeof(int) * data_size, std::ios::beg);
    bin_file.read(reinterpret_cast<char *>(*data), sizeof(int) * data_size);

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    // MPI initialization
    int numprocs;     // number of proccesses
    // int myid;         // current proc RANK
    // MPI_Status stat;  // routine status
    // MPI_Init(&argc, &(argv));
    // MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    // MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    // load random data
    int *data; // this is the data we're sorting
    unsigned long data_size;
    fill_array_from_binary_file(&data, argv[1], myid, numprocs, data_size);

    int sorted[data_size]; // this is our sorted array


    myid = 0;
    if(myid == 0) {
        printf("Sorted Data: \n")
        for(int i = 0; i < data_size; i++) {
            printf("%d \n", data[i]);
        }
    }

    // MPI_Finalize();
    return 0;
}
