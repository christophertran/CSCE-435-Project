#include <math.h>
#include <mpi.h>
#include <string.h>

#include <fstream>
#include <iostream>

#include "stdio.h"

#define ELEMENT_TAG 500

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

    int sorted[numprocs];    // this is our sorted array
    int indices[numprocs];   // final indices of all processes
    int elements[numprocs];  // elements held by all processes
    int number;              // a temp variable for every element in the input array
    int element;             // this is the element assigned to this process
    int final_index = 0;     // this is the index of the element assigned to this process
                             // in the final sorted array

    // start sorting
    if (myid == 0) {  // main proc
        printf("MAIN PROCESS\n");

        // load random data
        int *data;  // this is the data we're sorting
        unsigned long data_size;
        fill_array_from_binary_file(&data, argv[1], myid, numprocs, data_size);

        printf("numprocs: %d \n", numprocs);

        // print data
        for (int i = 0; i < numprocs; i++) {
            printf("%d: %d \n", i, data[i]);
        }

        for (int i = 0; i < numprocs; ++i) {  // i represents id of process
            number = data[i];

            // printf("i: %d\n", i);
            // printf("i != 0: %d\n", i != 0);

            // send value to corresponding proc's reg X
            if (i != 0) {  //if not the main process, send the number to process i
                // printf("Process %d Sending Element: %d to Process %d\n", myid, element, i);
                MPI_Send(&number, 1, MPI_INT, i, ELEMENT_TAG, MPI_COMM_WORLD);
                // printf("SEND SUCCESS\n");
            } else {
                element = number;  // if process is main, element is first one in input array
            }

            // decide if incrementing this processes/elements final index in the sorted array
            final_index += (element > number);
        }

        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
    } else {  // sub procs
        // printf("SUB PROCESS: %d\n", myid);
        MPI_Barrier(MPI_COMM_WORLD);
        // printf("SUB PROCESS POST BARRIER\n");

        MPI_Recv(&element, 1, MPI_INT, 0, ELEMENT_TAG, MPI_COMM_WORLD, &stat);
        MPI_Barrier(MPI_COMM_WORLD);

        // printf("Process %d Recieved Element: %d \n", myid, element);

        // do comparsion
        for (int i = 0; i < numprocs; ++i) {
            // MPI_Recv(&reg_y, 1, MPI_INT, myid - 1, REG_TAG_Y, MPI_COMM_WORLD, &stat);
            number = data[i];

            if (i != myid) {
                final_index += (element > number);
            }
        }
    }

    // wait for all index calculations
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Gather(&element, 1, MPI_FLOAT, elements, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Gather(&final_index, 1, MPI_FLOAT, indices, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    if (myid == 0) {
        // assign element to sorted array
        for (int i = 0; i < numprocs; i++) {
            sorted[indices[i]] = elements[i];
        }

        // print sorted array
        printf("\nSorted Data: \n");
        for (int i = 0; i < numprocs; i++) {
            printf("%d: %d \n", i, sorted[i]);
        }
    }

    MPI_Finalize();
    return 0;
}
