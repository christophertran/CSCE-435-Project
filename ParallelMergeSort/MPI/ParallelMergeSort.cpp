#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include "mpi.h"

//Uncomment below to see final sorted array
//#define COUT


using namespace std;

int fill_array_from_binary_file(int **data, char *binary_file, long rank, int count_processes, unsigned long &data_size) {
    ifstream bin_file(binary_file, ios::in | ios::binary);
    bin_file.seekg(0, ios::end);

    const long int count_all_bytes = bin_file.tellg();
    data_size = (count_all_bytes / sizeof(int) / count_processes);

        if (data_size < 1)
    {
        cout << "The amount of processes is higher than the amount of values. In this case not every process will become values." << std::endl;
        return EXIT_FAILURE;
    }
    else if ((count_all_bytes / sizeof(int)) % count_processes != 0)
    {
        cout << "The amount of values have to be even on every process. Otherwise the sorting would be incorrect." << std::endl;
        return EXIT_FAILURE;
    }


    *data = new int[data_size];

    bin_file.seekg(rank * sizeof(int) * data_size, ios::beg);
    bin_file.read(reinterpret_cast<char *>(*data), sizeof(int) * data_size);
    return EXIT_SUCCESS;
}

void merge(int *a, int *b, int l, int m, int r) {
	int h, i, j, k;
	h = l;
	i = l;
	j = m + 1;
	while((h <= m) && (j <= r)) {

		if(a[h] <= a[j]) {	
			b[i] = a[h];
			h++;	
			}	
		else {	
			b[i] = a[j];
			j++;	
			}	
		i++;
    }
	if(m < h) {
		for(k = j; k <= r; k++) {
			
			b[i] = a[k];
			i++;
			
		}	
	}	
	else {
		for(k = h; k <= m; k++) {	
			b[i] = a[k];
			i++;	
			}	
	}	
	for(k = l; k <= r; k++) {
		a[k] = b[k];	
	}	
}

void mergeSort(int *a, int *b, int l, int r) {
	int m;
	if(l < r) {	
		m = (l + r)/2;	
		mergeSort(a, b, l, m);
		mergeSort(a, b, (m + 1), r);
		merge(a, b, l, m, r);
		
	}		
}

void printList(int array[], int array_size, int myid, int numprocs, int *global_array) {
    if (myid == 0) {
        int n = array_size * numprocs;
        global_array = new int[n];

        MPI_Gather(array, array_size, MPI_INT, global_array, array_size, MPI_INT, 0, MPI_COMM_WORLD);

        //Make the final merge
        int *other_array = new int[n];
        mergeSort(global_array, other_array, 0, ( (array_size)*numprocs - 1));

        #ifdef COUT
            cout << "Sorted list: " << endl;
            for (int i = 0; i < n; i++)
            {
                cout << global_array[i] << " ";
            }
            cout << endl;
        #endif

        delete global_array, other_array;
    }
    else {
        MPI_Gather(array, array_size, MPI_INT, global_array, array_size, MPI_INT, 0, MPI_COMM_WORLD);
    }

}


int main(int argc, char** argv) {
    int numprocs;
    int myid;
    MPI_Status stat;

    //MPI Initialization
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    // [0]; Measure the total time it takes to ingest, sort, and gather sorted data.
    // [1]; Measure the total time it takes to gather sorted data.
    // [2]; Measure the individual data ingestion time of each process. (min, max, avg)
    // [3]; Measure the indvidual computation time of each process. (min, max, avg)
    double local_runtime[4];

    // TOTAL COMPUTATION STARTS HERE (Timing only matters for Rank 0 process)
    if (myid == 0)
    {
        local_runtime[0] = MPI_Wtime();
    }

    // INDIVIDUAL DATA INGESTION TIME STARTS HERE
    local_runtime[2] = MPI_Wtime();

    //Read data from binary file and store the values
    int *data;
    unsigned long data_size;
    int status = fill_array_from_binary_file(&data, argv[1], myid, numprocs, data_size);
    
    if (status == EXIT_FAILURE) {
        MPI_Finalize();
        return EXIT_FAILURE;
    }
    
    local_runtime[2] = MPI_Wtime() - local_runtime[2];
    // INDIVIDUAL DATA INGESTION TIME ENDS HERE



    // INDIVIDUAL COMPUTATION TIME STARTS HERE
    local_runtime[3] = MPI_Wtime();

    //Create a temporary array and perform the merge sort on each process
    int *tmp_array = new int[data_size];
    mergeSort(data, tmp_array, 0, (data_size - 1));

    int *sorted = NULL;

    local_runtime[3] = MPI_Wtime() - local_runtime[3];
    // INDIVIDUAL COMPUTATION TIME ENDS HERE
    



    // TOTAL GATHERING TIME STARTS HERE (Timing only matters for Rank 0 process)
    if (myid == 0)
    {
        local_runtime[1] = MPI_Wtime();
    }

    //Gather data and perform final merge, and print if requested
    printList(data, data_size, myid, numprocs, sorted);

    if (myid == 0)
    {
        local_runtime[1] = MPI_Wtime() - local_runtime[1];
    }
    // TOTAL GATHERING TIME ENDS HERE (Timing only matters for Rank 0 process)

    delete data, tmp_array;

    if (myid == 0)
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

    if (myid == 0)
    {
        std::cout << std::fixed;
        std::cout << "==========\n";
        std::cout << "Processes: " << numprocs << "\n";
        std::cout << "Input Size: " << data_size * numprocs << "\n";
        std::cout << "==========\n";
        std::cout << "Total Computation: " << local_runtime[0] << "\n";
        std::cout << "Total Gathering: " << local_runtime[1] << "\n";
        std::cout << "==========\n";
        std::cout << "Individual Data Ingestion Min: " << global_min[2] << "\n";
        std::cout << "Individual Data Ingestion Max: " << global_max[2] << "\n";
        std::cout << "Individual Data Ingestion Avg: " << global_avg[2] / numprocs << "\n";
        std::cout << "==========\n";
        std::cout << "Individual Computation Min: " << global_min[3] << "\n";
        std::cout << "Individual Computation Max: " << global_max[3] << "\n";
        std::cout << "Individual Computation Avg: " << global_avg[3] / numprocs << std::endl;
    }


    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();


    return EXIT_SUCCESS;
}