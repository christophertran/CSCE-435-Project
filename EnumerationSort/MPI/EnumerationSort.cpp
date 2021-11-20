/*
 ***********************************************************************
 *
 *        @version  1.0
 *        @date     03/07/2014 11:52:40 AM
 *        @author   Fridolin Pokorny <fridex.devel@gmail.com>
 * 
 *      Adapted by Rohit Barichello for use in CSCE 435 at Texas A&M University
 *
 ***********************************************************************
 */

#include <math.h>
#include <mpi.h>

#include "stdio.h"

// these are tests
// #include <algorithm>
#include <string.h>

#include <fstream>
#include <iostream>

#define REG_EMPTY (-1)
#define REG_TAG_X 256
#define REG_TAG_Y 257
#define REG_TAG_Z 258
#define REG_TAG_C 259
#define REG_TAG_ZC 260

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
    int numprocs;     // number of procecs
    int myid;         // current proc RANK
    MPI_Status stat;  // routine status

    int reg_x;    // register X
    int reg_y;    // register Y
    int reg_z;    // register Z
    int compare;  // comparsion register
    size_t z_count;

    // init
    MPI_Init(&argc, &(argv));
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    compare = 0;
    z_count = 0;
    reg_x = REG_EMPTY;
    reg_y = REG_EMPTY;
    reg_z = REG_EMPTY;

    double time;

    // x reg is for a processes current value and y is comparison register
    // z is final array register
    // c is count register
    if (myid == 0) {  // main proc
        // time = MPI_Wtime();
        int *data;
        unsigned long data_size;
        fill_array_from_binary_file(&data, argv[1], myid, numprocs, data_size);
        // printf("Data Ingestion Time: %f\n", MPI_Wtime() - time);

        int number;

        for (int i = 0; i < numprocs; ++i) {  // i represents id of process
            number = data[i];

            // send value to corresponding proc's reg X
            if (i != 0) {  //if not the main process, send the number to process i
                MPI_Send(&number, 1, MPI_INT, i, REG_TAG_X, MPI_COMM_WORLD);
            } else {
                reg_x = number;  // if process is main, set x reg to current number
            }

            reg_y = number;              // set y reg to current number and compare with x reg
            compare += (reg_x < reg_y);  // if comparision number is greater than this processes
                                         // value, increment comparison

            // send it to my neighbour's Y reg
            MPI_Send(&reg_y, 1, MPI_INT, myid + 1, REG_TAG_Y, MPI_COMM_WORLD);
        }
    } else {  // sub procs
        MPI_Recv(&reg_x, 1, MPI_INT, 0, REG_TAG_X, MPI_COMM_WORLD, &stat);

        // do comparsion
        for (int i = 0; i < numprocs; ++i) {
            MPI_Recv(&reg_y, 1, MPI_INT, myid - 1, REG_TAG_Y, MPI_COMM_WORLD, &stat);

            if (i < numprocs) {
                if (reg_x != REG_EMPTY && reg_x != REG_EMPTY) {
                    compare += (reg_x < reg_y);
                }
            }

            if (myid != numprocs - 1) {
                MPI_Send(&reg_y, 1, MPI_INT, myid + 1, REG_TAG_Y, MPI_COMM_WORLD);
            }
        }
    }

    // wait for all processors to calculate the element ranks;
    MPI_Barrier(MPI_COMM_WORLD);

    int value;  // value to send

    // move results to register Z depending on compare register value
    time = MPI_Wtime();
    for (int i = 0; i < numprocs; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (myid == i) {
            if (compare != myid) {
                value = compare;
            } else {
                value = REG_EMPTY;
                reg_z = reg_x;
                z_count++;
            }
        } else {
            value = REG_EMPTY;
        }

        // broadcast index of proc to wait for a msg
        MPI_Bcast(&value, 1, MPI_INT, i, MPI_COMM_WORLD);  // we send or receive all values to all processes.

        // send x value to reg z
        if (myid == i && compare != myid) {
            MPI_Send(&reg_x, 1, MPI_INT, compare, REG_TAG_Z, MPI_COMM_WORLD);
        }

        // I am going to receive the reg Z value
        if (value == myid) {
            MPI_Recv(&reg_z, 1, MPI_INT, i, REG_TAG_Z, MPI_COMM_WORLD, &stat);
            z_count++;
        }
    }

    time = MPI_Wtime() - time;
    double global_sum;
    MPI_Reduce(&time, &global_sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (myid == 0) {
        printf("Individual Gathering Average: %f\n", global_sum / numprocs);
    }

    // print the result
    for (int i = 0; i < numprocs; ++i) {
        if (myid == 0) {
            if (reg_z != REG_EMPTY) {
                for (size_t i = 0; i < z_count; i++) {
                    // printf(" %d \n", reg_z);
                    ;
                }
            }
        }

        if (myid != 0) {
            MPI_Send(&reg_z, 1, MPI_INT, myid - 1, REG_TAG_Z, MPI_COMM_WORLD);
            MPI_Send(&z_count, 1, MPI_INT, myid - 1, REG_TAG_ZC, MPI_COMM_WORLD);
        }
        if (myid != numprocs - 1) {
            MPI_Recv(&reg_z, 1, MPI_INT, myid + 1, REG_TAG_Z, MPI_COMM_WORLD, &stat);
            MPI_Recv(&z_count, 1, MPI_INT, myid + 1, REG_TAG_ZC, MPI_COMM_WORLD, &stat);
        }
    }

    MPI_Finalize();
    return 0;
}
