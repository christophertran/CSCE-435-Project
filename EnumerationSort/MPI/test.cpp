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
    int myid;         // current proc RANK
    MPI_Status stat;  // routine status
    MPI_Init(&argc, &(argv));
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    // load random data
    int *data;  // this is the data we're sorting
    unsigned long data_size;
    fill_array_from_binary_file(&data, argv[1], myid, numprocs, data_size);

    int sorted[data_size];  // this is our sorted array
    int number;             // a temp variable for every element in the input array
    int element;            // this is the element assigned to this process
    int final_index = 0;    // this is the index of the element assigned to this process
                            // in the final sorted array

    // start sorting
    if (myid == 0) {  // main proc
        printf("MAIN PROCESS\n");
        printf("Num Process: %d \n", numprocs);
        for (int i = 0; i < data_size; ++i) {  // i represents id of process
            number = data[i];

            // send value to corresponding proc's reg X
            if (i != 0) {  //if not the main process, send the number to process i
                MPI_Send(&number, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            } else {
                element = number;  // if process is main, element is first one in input array
            }

            // decide if incrementing this processes/elements final index in the sorted array
            final_index += (element > number);
        }
        printf("\n");
    } else {  // sub procs
        printf("SUB PROCESS\n");
        MPI_Recv(&element, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);

        // do comparsion
        for (int i = 0; i < data_size; ++i) {
            // MPI_Recv(&reg_y, 1, MPI_INT, myid - 1, REG_TAG_Y, MPI_COMM_WORLD, &stat);
            number = data[i];

            if (i != myid) {
                final_index += (element > number);
            }
        }
    }

    // wait for all processses to calculate their element's final index
    MPI_Barrier(MPI_COMM_WORLD);

    // assign element to sorted array
    sorted[final_index] = element;

    MPI_Barrier(MPI_COMM_WORLD);

    if (myid == 0) {
        printf("Sorted Data: \n");
        for (int i = 0; i < data_size; i++) {
            printf("%d: %d \n", i, sorted[i]);
        }
    }

    MPI_Finalize();
    return 0;
}
