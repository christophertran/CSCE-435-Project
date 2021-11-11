/*
 ***********************************************************************
 *
 *        @version  1.0
 *        @date     03/07/2014 11:52:40 AM
 *        @author   Fridolin Pokorny <fridex.devel@gmail.com>
 *
 ***********************************************************************
 */

#include <mpi.h>
#include <math.h>

#include "stdio.h"

#define REG_EMPTY (-1)
#define REG_TAG_X 256
#define REG_TAG_Y 257
#define REG_TAG_Z 258
#define REG_TAG_C 259
#define REG_TAG_ZC 260

int main(int argc, char *argv[]) {
    int numprocs;     // number of procecs
    int myid;         // current proc RANK
    MPI_Status stat;  // routine status

    int array[5] = [4,6,3,8,1];

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

    if (myid == 0) {  // main proc
        int number;

        for (int i = 0; fin.good() && i < {{{LENGTH OF THE ARRAY}}}; ++i) {
            number = array[i]

            printf(" %d", number);

            // send value to corresponding proc's reg X
            if (i != 0) {
                MPI_Send(&number, 1, MPI_INT, i, REG_TAG_X, MPI_COMM_WORLD);
            }
            else {
                reg_x = number;
            }

            reg_y = number;
            compare += (reg_x < reg_y);

            // send it to my neighbour's Y reg
            MPI_Send(&reg_y, 1, MPI_INT, myid + 1, REG_TAG_Y, MPI_COMM_WORLD);
        }
        printf("\n");
    } 
    else {  // sub procs
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
        MPI_Bcast(&value, 1, MPI_INT, i, MPI_COMM_WORLD);

        if (myid == i && compare != myid) {
            MPI_Send(&reg_x, 1, MPI_INT, compare, REG_TAG_Z, MPI_COMM_WORLD);
        }

        // I am going to receive the reg Z value
        if (value == myid) {
            MPI_Recv(&reg_z, 1, MPI_INT, i, REG_TAG_Z, MPI_COMM_WORLD, &stat);
            z_count++;
        }
    }

    // print the result
    for (int i = 0; i < numprocs; ++i) {
        if (myid == 0) {
            if (reg_z != REG_EMPTY) {
                for (size_t i = 0; i < z_count; i++)
                    printf(" %d \n", reg_z);
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
