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

    ```txt
    ```

4. Hyper Quick Sort (MPI)

    ```txt
    ```

## 3. _due 11/12_ Evaluation plan - what and how will you measure and compare

Group project idea you are welcome to use: Choose 3 parallel sorting algorithms, implement in MPI and CUDA.  Examine and compare performance in detail (computation time, communication time, how much data is sent) on a variety of inputs: sorted, random, reverse, sorted with 1% perturbed, etc.  Strong scaling, weak scaling, GPU performance.

We will compare the performance of 4 MPI sorting algorithms. We will be measuring not only the total computation/sorting time, but we will also consider things like the communication time, and how much data is sent between nodes/processes. 

In addition to measuring these items with various size and random inputs, we will also be measuring with various sizes of sorted and reversed inputs.

Another source of variation that we will be testing is varying the number of nodes/processes to look for either strong or weak scaling.

Overall we will be looking for trends in the sorting algorithms with various inputs and input sizes as well as with various numbers of nodes/processes.

## 4. _due 11/19_ Performance evaluation

Include detailed analysis of computation performance, communication performance.

Include figures and explanation of your analysis.

![alt text](image.jpg)

## 5. _due 12/1_ Presentation, 5 min + questions

- Power point is ok

## 6. _due 12/8_ Final Report

Example [link title](https://) to preview _doc_.
