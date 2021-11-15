#include "mpi.h"
#include<stdio.h>
#include <stdlib.h>
#include "math.h"
#include <stdbool.h>
#include <string.h>
#include <fstream>
#include <iostream>
#define SIZE 1000000

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

int partition(int *arr, int low, int high){
    // Divide the array into two partitions (Lower than pivot & Higher than pivot)
    // and returns the Pivot index in the array

    int pivot = arr[high];
    int i = (low - 1);
    int j,temp;
    for (j = low; j <= high - 1; j++){
        if(arr[j] < pivot){
            i++;
            temp = arr[i];  
            arr[i] = arr[j];
            arr[j] = temp;	
        }
    }
    temp = arr[i+1];  
    arr[i+1] = arr[high];
    arr[high] = temp; 
    return (i + 1);
}

int hoare_partition(int *arr, int low, int high){
    // Pivot starts at the middle point 
    // Divide the array into two partitions (Lower than pivot & Higher than pivot)
    // and returns the Pivot index in the array

    int middle = floor((low+high)/2);
    int pivot = arr[middle];
    int j, temp;
    // move pivot to the end
    temp = arr[middle];  
    arr[middle] = arr[high];
    arr[high] = temp;

    int i = (low - 1);
    for (j = low; j <= high - 1; j++){
        if(arr[j] < pivot){
            i++;
            temp = arr[i];  
            arr[i] = arr[j];
            arr[j] = temp;	
        }
    }
    // move pivot back
    temp = arr[i+1];  
    arr[i+1] = arr[high];
    arr[high] = temp; 

    return (i + 1);
}

void quicksort(int *number,int first,int last){
    // Simple quicksort
    if(first < last){
        int pivot_index = partition(number, first, last);
        quicksort(number,first,pivot_index-1);
        quicksort(number,pivot_index+1,last);
    }
}

int quicksort_recursive(int* arr, int arrSize, int currProcRank, int maxRank, int rankIndex) {
    // quicksort that works on sharing the subarrays 
    MPI_Status status;

    // Calculate the rank of the Cluster 
    int shareProc = currProcRank + pow(2, rankIndex);
    rankIndex++;

    // If no Cluster is available, sort with simple quicksort
    if (shareProc > maxRank) {
        MPI_Barrier(MPI_COMM_WORLD);
	    quicksort(arr, 0, arrSize-1 );
        return 0;
    }
    // Divide array in two parts with the pivot in between
    int j = 0;
    int pivotIndex;
    pivotIndex = hoare_partition(arr, j, arrSize-1 );

    // Send partition based on size (always send the smaller part), 
    // Sort the remaining partitions,
    // Receive sorted partition
    if (pivotIndex <= arrSize - pivotIndex) {
        MPI_Send(arr, pivotIndex , MPI_INT, shareProc, pivotIndex, MPI_COMM_WORLD);
	    quicksort_recursive((arr + pivotIndex+1), (arrSize - pivotIndex-1 ), currProcRank, maxRank, rankIndex); 
        MPI_Recv(arr, pivotIndex , MPI_INT, shareProc, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }
    else {
        MPI_Send((arr + pivotIndex+1), arrSize - pivotIndex-1, MPI_INT, shareProc, pivotIndex + 1, MPI_COMM_WORLD);
        quicksort_recursive(arr, (pivotIndex), currProcRank, maxRank, rankIndex);
        MPI_Recv((arr + pivotIndex+1), arrSize - pivotIndex-1, MPI_INT, shareProc, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }
}


int main(int argc, char *argv[]) {
    int size, rank;
    int *data = NULL;
    unsigned long data_size;

    // Start Parallel Execution
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // array generation 
    if(rank==0){ 
        int cur_status = fill_array_from_binary_file(&data, argv[1], rank, size, data_size);

        if (cur_status == EXIT_FAILURE) {
            MPI_Finalize();
            return EXIT_FAILURE;
        }
	}

    int array_size = (int) data_size;
    int unsorted_array[array_size]; 

    // Filling unsorted array with the data 
    if(rank==0){
        int j = 0;
        for (j = 0; j < array_size; ++j) {
            unsorted_array[j] = data[j];
        }
	}
    
    // Calculate in which layer of the tree each Cluster belongs
    int rankPower = 0;
    while (pow(2, rankPower) <= rank){
        rankPower++;
    }
    // Wait for everything to reach this current point 
    MPI_Barrier(MPI_COMM_WORLD);
    double start_timer, finish_timer;
    if (rank == 0) {
	    start_timer = MPI_Wtime();
        // Master starts the Execution and always runs 
        // recursively, keeping the left bigger half
        quicksort_recursive(unsorted_array, array_size, rank, size - 1, rankPower);    
    }else{ 
        // All other Clusters wait for their subarray to arrive,
        // they sort it and they send it back.
        MPI_Status status;
        int subarray_size;
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        // Capturing size of the array to receive
        MPI_Get_count(&status, MPI_INT, &subarray_size);
	    int source_process = status.MPI_SOURCE;     
        int subarray[subarray_size];
        MPI_Recv(subarray, subarray_size, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        quicksort_recursive(subarray, subarray_size, rank, size - 1, rankPower);
        MPI_Send(subarray, subarray_size, MPI_INT, source_process, 0, MPI_COMM_WORLD);
    };
    
    if(rank==0){
        finish_timer = MPI_Wtime();
	    printf("Total time to sort data: %2.2f sec \n", finish_timer-start_timer);

        // print the sorted values 
        printf("Sorted array using Hyper Quick Sort... \n");
        
        int i = 0;
        for(i = 0; i < array_size - 1; i++) { 
            std::cout << unsorted_array[i] << endl;
        }        
    }
       
    MPI_Finalize();
    // End of Parallel Execution
    return 0;
}