
## Pseudocode (and some notes) for the Merge Sort algorithm.

So the basis of this is that we use the divide & conquer approach. <br />
We break the problem down into smaller parts using recursion and deal with those smaller parts. <br />
After that, we merge all the small parts together.

### Pseudocode for Merging

Suppose we have two sub-arrays that each contain 5 elements.
A = { 2, 8, 3, 15 } <br />
B = {6, 12, 5, 1} <br />

We want to merge it into a single sorted list. <br />
I will define a couple of other parameters that help us in the merging process.

C - target array in which we will store the final result

n - length of A sub-array <br />
m - length of B sub-array <br />

i - index of the first element of sub-array A <br />
j - index of the first element of sub-array B <br />
k - index of the first element in the target array where we want to store the sorted list <br />

i = 1; j = 1; k = 1; <br />

Pseudocode: <br />
```
Merge(A, B, n, m) {
    while(i <= n && j <= m) {
        if(A[i] < B[j]) 
            C[k] = A[i]
            i++
            k++

        else 
            C[k] = B[j]
            j++
            k++

    }

    If there are any remaining elements, copy them over to the target array. 
        for(; i <= n; i++) {
            C[k] = A[i]
            k++
        }

    for(; j <= m; j++) {
        C[k] = B[j]
        k++
    }
}
```











