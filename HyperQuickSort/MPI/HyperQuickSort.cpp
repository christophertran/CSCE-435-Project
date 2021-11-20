#include <iostream>
#include <sys/time.h>
#include <mpi.h>
#include <cstdlib>

using namespace std;

void swap(int *data, int i, int j) {
    int temp = data[i];
    data[i] = data[j];
    data[j] = temp;
}

int partition(int *data, int start, int end) {
    if (start >= end) return 0;

    int pivotValue = data[start];
    int low = start;
    int high = end - 1;
    while (low < high) {
    while (data[low] <= pivotValue && low < end) low++;
    while (data[high] > pivotValue && high > start) high--;
    if (low < high) swap(data, low, high);
    }
    swap(data, start, high);

    return high;
}

void quicksort(int *data, int start, int end) {
    if  (end-start+1 < 2) return;

    int pivot = partition(data, start, end);

    quicksort(data, start, pivot);
    quicksort(data, pivot+1, end);
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;

    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);

    // [0]; Measure the total time it takes to ingest, sort, and gather sorted data.
    double local_runtime[0];
    double gather_runtime[0];

    // TOTAL COMPUTATION STARTS HERE (Timing only matters for Rank 0 process)
    if (rank == 0)
    {
        local_runtime[0] = MPI_Wtime();
    }

    if (argc < 2) {
        if (rank == 0)
            cout << "Usage: mpiqsort num_of_numbers" << endl;
        exit(0);
    }

    int length = atoi(argv[1]);
    srand(time(NULL));
    int number;
    int *data = new int[length];	// Big enough to hold it all
    int i;
    std::string word = argv[2];

    if(word == "RAND"){ 
        // cout << "Making random list" << endl;
        for (i = 0; i < length; i++){
            data[i] = rand();
        }
    }
    else if(word == "SORT"){
        // cout << "Making sorted list" << endl;
        for (i = 0; i < length; i++){
            data[i] = i;
        }
    }
    else if(word == "REV"){
        // cout << "Making reversed list" << endl;
        number = length - i;
        for (i = 0; i < length; i++){
            data[i] = length - i;
        }
    }

    MPI_Status status;
    // Send all of the data to processor 0
    if (rank == 0) {
        for (i=1; i<size; i++)
            MPI_Recv(data+i*length/size, length/size, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }
    else {
        MPI_Send(data, length/size, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    // Time everything after exchange of data until sorting is complete
    timeval start, end;
    gettimeofday(&start, 0);

    // TOTAL GATHERING TIME STARTS HERE (Timing only matters for Rank 0 process)
    if (rank == 0)
    {
        gather_runtime[0] = MPI_Wtime();
    }

    // Do recursive quicksort starting at processor 0 and spreading out recursive calls to other machines
    int s;
    int localDataSize =  length;
    int pivot;
    for (s=size; s > 1; s /= 2) {
        if (rank % s == 0) {
            pivot = partition(data, 0, localDataSize);

            // Send everything after the pivot to processor rank + s/2 and keep up to the pivot
            MPI_Send(data+pivot, localDataSize - pivot, MPI_INT, rank + s/2, 0, MPI_COMM_WORLD);
            localDataSize = pivot;
        }
        else if (rank % s == s/2) {
            // Get data from processor rank - s/2
            MPI_Recv(data, length, MPI_INT, rank - s/2, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            // How much data did we really get?
            MPI_Get_count(&status, MPI_INT, &localDataSize);
        }
    }

    // Perform local sort
    quicksort(data, 0, localDataSize);

    if (rank == 0)
    {
        gather_runtime[0] = MPI_Wtime() - gather_runtime[0];
    }
    // TOTAL GATHERING TIME ENDS HERE (Timing only matters for Rank 0 process)

    if (rank == 0)
    {
        local_runtime[0] = MPI_Wtime() - local_runtime[0];
    }
    // TOTAL COMPUTATION ENDS HERE (Timing only matters for Rank 0 process)
    
    // Measure elapsed time
    if (rank == 0)
    {
        std::cout << std::fixed;
        std::cout << "==========\n";
        std::cout << "Processes: " << size << "\n";
        std::cout << "Input Size: " << length << "\n";
        std::cout << "==========\n";
        std::cout << "Total Computation: " << local_runtime[0] << "\n"; 
        std::cout << "Total Gathering: " << local_runtime[1] << "\n";
        std::cout << "==========\n";
    }

    MPI_Finalize();
}