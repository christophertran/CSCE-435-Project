# ParallelComputing CSCE 435 Group project

[TAMU GitHub Repo](https://github.tamu.edu/rohitbarichello/csce435project)

## 1. _due 10/27_ Group members

1. Rohit Barichello
2. Christopher Tran
3. Vicente Trevino
4. Kelton Chesshire

---

## 2. _due 11/3_ Project topic

We'll be implementing 3 different parallel sorting algorithms in MPI and CUDA and comparing them.

We'll also be measuring communication time, computation time, and how much data is sent, with varied inputs. Our inputs will include sorted, random, and reversed lists.

Our main communication method will be through Discord.

## 2. _due 11/3_ Brief project description (what algorithms will you be comparing and on what architectures)

[Source - Tutorials Point](https://www.tutorialspoint.com/parallel_algorithm/parallel_algorithm_sorting.htm)

1. Odd-Even Transposition Sort (MPI)

    ```text
    procedure ODD-EVEN_PAR (n) 

    begin 
    id := process's label 
        
    for i := 1 to n do 
    begin 
        
        if i is odd and id is odd then 
            compare-exchange_min(id + 1); 
        else 
            compare-exchange_max(id - 1);
                
        if i is even and id is even then 
            compare-exchange_min(id + 1); 
        else 
            compare-exchange_max(id - 1);
                
    end for
        
    end ODD-EVEN_PAR
    ```

2. Parallel Merge Sort (MPI)

    ```text
    procedure PARALLEL_MERGE_SORT(id, n, data, newdata)

    begin
    data = sequentialmergesort(data)
        
        for dim = 1 to n
            data = parallelmerge(id, dim, data)
        endfor
            
    newdata = data
    end PARALLEL_MERGE_SORT
    ```

3. Enumeration Sort (MPI)

    ```text
    procedure ENUM_SORTING (n)

    begin
    for each process P1,j do
        C[j] := 0;
            
    for each process Pi, j do
        
        if (A[i] < A[j]) or A[i] = A[j] and i < j) then
            C[j] := 1;
        else
            C[j] := 0;
                
    for each process P1, j do
        A[C[j]] := A[j];
            
    end ENUM_SORTING
    ```

4. Hyper Quick Sort (MPI)

    ```text
    procedure HYPER_QUICKSORT (B, n)

    begin

    id := process’s label;
        
    for i := 1 to d do
        begin
        x := pivot;
        partition B into B1 and B2 such that B1 ≤ x < B2;
        if ith bit is 0 then
            
        begin
            send B2 to the process along the ith communication link;
            C := subsequence received along the ith communication link;
            B := B1 U C;
        endif
        
        else
            send B1 to the process along the ith communication link;
            C := subsequence received along the ith communication link;
            B := B2 U C;
            end else
        end for
            
    sort B using sequential quicksort;
        
    end HYPER_QUICKSORT
    ```

---

## 3. _due 11/12_ Pseudocode for each algorithm and implementation

[Source - Tutorials Point](https://www.tutorialspoint.com/parallel_algorithm/parallel_algorithm_sorting.htm)

1. Odd-Even Transposition Sort (MPI)

    [Source - selkie-macalester.org](http://selkie-macalester.org/csinparallel/modules/MPIProgramming/build/html/oddEvenSort/oddEven.html)

    "Odd-Even Transposition Sort is based on the Bubble Sort technique. It compares two adjacent numbers and switches them, if the first number is greater than the second number to get an ascending order list. Odd-Even transposition sort operates in two phases − odd phase and even phase. In both the phases, processes exchange numbers with their adjacent number in the right."

    The Odd-Even Transposition Sort was implemented using MPI. The data being sorted is read from a binary file that contains *x* random numbers. The way this algorithm is written, on startup, each process is assigned a rank and based on this rank and the total number of processes, the processes each will read a chunk of the numbers from the binary file. The size of the chunk and the chunk each process reads is determined by their rank and the total number of processes.

    Now the actual Odd-Even portion of the sort occurs. Each process will sort locally its chunk of values using quick sort. Then based on whether it is the Odd phase or the Even phase, each process will pair up with a partner process to swap data is necessary. The swapping continues as many times as there are processes. This is to ensure that we swap all local process data if necessary and we end up with the final data as being sorted.

2. Parallel Merge Sort (MPI)

    Parallel Merge Sort is a parallel version of the well-known mergesort algorithm. This algorithm assumes that the sequence to be sorted is distributed and so generates a distributed sorted sequence. To simplify things, we distribute the data evenly among the processors, and make sure that the processors are an integer power of two.

    In the MPI implementation, each process first reads from a binary file containing a predetermined amount of random numbers. Each process takes a specific chunk from the amount of random numbers assigned by rank, with the size of the chunk being determined by the rank and the total number of processes. Once this occurs, the merge begins. Each process performs a mergesort on their specific sections of the original data set, which is then combined back into a larger array on the primary process that is merged again. Then the sorted array is output to the console.

3. Enumeration Sort (MPI)

    The enumeration sort is a parallel sorting algorithm that comparesevery element of an array to every other element in the array. Each element is assigned a count. An element's count is incremented by one when compared to an element with a smaller value. At the end of the comparisons, each element is placed in an array at the index of its count. This algorithm indexes elements based on how many elements they're greater than.

    This would usually be a naive approach to sorting. However, because we're leveraging multiple processes, the task is really only slightly less efficient than an O(n) algorithm.

    The MPI implementation starts by having the main process send each element in the array to all other processes. Each subprocess then takes the value it recieves from the main process and compares it to the entire array using a for-loop. Each process has its count variable which is incremented as the comparison takes place.

    An MPI Barrier is used to stall all processes until all comparison is done. Then, each process broadcasts it's count variable and element value. These pairs of data are compiled into a sorted list by the main process and printed.

4. Hyper Quick Sort (MPI)

    Hyper Quick Sort is an implementation of the quick sort algorithm using parallelism. The fundamental of quicksort is choosing a value and partitioning the input data set to two subsets. One subset contains input data smaller in size than the chosen value and the other contains input data greater than the chosen value. This chosen value is called the pivot value. And in each step, these divided data sets are sub-divided choosing pivots from each set. Quicksort stop conditions are met when no sub division is possible.

    The Hyper Quick Sort algorithm was implemented using MPI. Initially, the data is read in from a binary file with a predetermined amount of random numbers. Once the data is stored within an array the head process will start quicksort with each process waiting to receive a subarray. Once the process receives a subarray it divides the array into smaller parts and distributes it to available processes. If there aren't any other available, then it sorts it sequentially. It then sends back the subarray.

## 3. _due 11/12_ Evaluation plan - what and how will you measure and compare

We will compare the performance of 4 MPI sorting algorithms. We will be measuring not only the total computation/sorting time, but we will also consider things like the communication time, and how much data is sent between nodes/processes.

In addition to measuring these items with various size and random inputs, we will also be measuring with various sizes of sorted and reversed inputs.

Another source of variation that we will be testing is varying the number of nodes/processes to look for either strong or weak scaling.

Overall we will be looking for trends in the sorting algorithms with various inputs and input sizes as well as with various numbers of nodes/processes.

- Input sizes: 2,560; 25,600; 256,000; 2,560,000;
- Process sizes: 8; 16; 32; 64; 128; 256;
- Input types:
  - Random
  - Sorted
  - Reversed (Sorted)
- Measurement points:
  - Total computation time
    - Measure the total time it takes to ingest, sort, and gather sorted data.
  - Total data gathering time
    - Measure the total time it takes to gather sorted data.
  - Individual computation time (min, max, avg)
    - Measure the indvidual computation time of each process.
  - Individual data ingestion time (min, max, avg)
    - Measure the individual data ingestion time of each process.

Graphs that we should include for each algorithm...

- For each input size (4) and for each input type (3) we should include a graph with each measurement point (4) for each number of processes (6).
- This means, overall, for each input size, we should have 3 graphs. This is a total of 12 graphs per sorting algorithm and 48 graphs for the project.
  - Random 2,560
  - Random 25,600
  - ...
  - Sorted 2,560
  - Sorted 25,600
  - ...
  - Reversed (Sorted) 2,560,000

## 4. _due 11/19_ Performance evaluation

3. Enumeration Sort (MPI)
For the enumeration sort, we were only able to demonstrate weak scaling due to the way the algorithm 
is implemented. For a given problem size, the algorithm, implemented as it is in this project, must
have the same number of processes. This only allows for varying the number of processors for a fixed
problem size. 

![](https://drive.google.com/uc?export=view&id=1wv0d9G2kzXlNKuROb2y9eMiNiMzJh1KH)
This first figure demonstrates a steady increase in the number 
![](https://drive.google.com/uc?export=view&id=1S2sO6D0onDYCk9gEOdjlfpgR3MzTiQBQ)
![](https://drive.google.com/uc?export=view&id=1icIA1lt5Kmbuz1fVLcpdtcFcAUEj7d3J)
![](https://drive.google.com/uc?export=view&id=1MpyprdqpUckupTT7SYmeotPKHM4fBsrF)

Include detailed analysis of computation performance, communication performance.

Include figures and explanation of your analysis.

## 5. _due 12/1_ Presentation, 5 min + questions

- Power point is ok

## 6. _due 12/8_ Final Report

Example [link title](https://) to preview _doc_.
