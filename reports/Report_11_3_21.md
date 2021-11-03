# Report 11/3/2021

## Group members
1. Rohit Barichello
2. Christopher Tran
3. Vicente Trevino
4. Kelton Chesshire

## Communication Method
We'll be using Discord as our main communication method

## Topic
We'll be implementing different parallel sorting algorithms in MPI and CUDA and comparing them

We'll be measuring communication time, computation time, and how much data is sent, with varied inputs. Our inputs will include sorted, random, and reversed lists, among others.

## Algorithms and pseudocode
1. odd-even transposition sort
        
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
2. parallel merge sort
        
        procedureparallelmergesort(id, n, data, newdata)

        begin
        data = sequentialmergesort(data)
            
            for dim = 1 to n
                data = parallelmerge(id, dim, data)
            endfor
                
        newdata = data
        end
3. enum sort:

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

