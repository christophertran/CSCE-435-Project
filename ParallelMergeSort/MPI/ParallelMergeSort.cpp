#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include "mpi.h"

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

int main(int argc, char** argv) {
    int numprocs;
    int myid;
    MPI_Status stat;

    //MPI Initialization
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    //Read data from binary file and store the values
    int *data;
    unsigned long data_size;
    fill_array_from_binary_file(&data, argv[1], myid, numprocs, data_size);
    
    if (myid == 0) {
        cout << "Number of ranks: " << numprocs << endl;
        cout << "Data size: " << data_size << endl;
        cout << "Unsorted data array: " << endl;
        for (int a = 0; a < data_size; a++) {
            cout << data[a] << " ";
        }
        cout << endl;
    }
    
    //Divide the array into equal-sized chunks
    int divided_size = (int) data_size / numprocs;

    //Create a sub-array and send them to each process
    int *sub_array = (int*)malloc(divided_size * sizeof(int));
    MPI_Scatter(data, divided_size, MPI_INT, sub_array, divided_size, MPI_INT, 0, MPI_COMM_WORLD);

    //Create a temporary array and perform the merge sort on each process
    int *tmp_array = (int*)malloc(divided_size*sizeof(int));
    mergeSort(sub_array, tmp_array, 0, (divided_size - 1));

    //Gather sorted sub-arrays into one
    int *sorted = NULL;
    if(myid == 0) {
        sorted = (int*)malloc(data_size*sizeof(int));
    }
    MPI_Gather(sub_array, divided_size, MPI_INT, sorted, divided_size, MPI_INT, 0, MPI_COMM_WORLD);

    //Make the final merge
    if (myid == 0 ){
        int *other_array = (int*)malloc(data_size*sizeof(int));
        mergeSort(sorted, other_array, 0, ( (int) data_size - 1));

        //Display the sorted array
        cout << "Final Array using MPI Parallelism: ";
        for(int i = 0; i < (int) data_size; i++) {
            cout << sorted[i] << " ";
        }
        cout << "\n" << endl;

        free(sorted);
        free(other_array);
    }

    free(sub_array);
    free(tmp_array);
    delete(data);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}