# ParallelComputing CSCE 435 Group project

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

[Source](https://www.tutorialspoint.com/parallel_algorithm/parallel_algorithm_sorting.htm)

1. Odd-Even Transposition Sort (MPI + CUDA)

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

2. Parallel Merge Sort (MPI + CUDA)

    ```text
    procedureparallelmergesort(id, n, data, newdata)

    begin
    data = sequentialmergesort(data)
        
        for dim = 1 to n
            data = parallelmerge(id, dim, data)
        endfor
            
    newdata = data
    end
    ```

3. Enumeration Sort (MPI + CUDA)

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

---

## 3. _due 11/12_ Pseudocode for each algorithm and implementation

## 3. _due 11/12_ Evaluation plan - what and how will you measure and compare

For example:

- Effective use of a GPU (play with problem size and number of threads)
- Strong scaling to more nodes (same problem size, increase number of processors)
- Weak scaling (increase problem size, increase number of processors)

## 4. _due 11/19_ Performance evaluation

Include detailed analysis of computation performance, communication performance.

Include figures and explanation of your analysis.

![alt text](image.jpg)

## 5. _due 12/1_ Presentation, 5 min + questions

- Power point is ok

## 6. _due 12/8_ Final Report

Example [link title](https://) to preview _doc_.
