/*
 * =====================================================================================
 *
 *       Filename:  merge_sort.c
 *
 *    Description: Recursive merge sort algorithm 
 *
 *        Version:  1.0
 *        Created:  09/23/2024 05:13:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  specter, 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Need to use memcpy() later on


void merge_sort(int l, int h, int *target, int *source);
void merge(int lower, int middle, int higher, int *target_array, int *source_array); 

int main(void) {
	// we need two arrays
	int array_size = 8;
	int source_arr[] = { 3,7,9,5,6,4,8,2 };
	int target_arr[array_size]; 
	merge_sort(0, 7, target_arr, source_arr);
	printf("Unsorted array: "); 
	for(int i = 0; i < array_size; i++) {
		printf("%d ", source_arr[i]);
	}
	printf("\n"); 

	printf("Sorted array: "); 
	for(int j = 0; j < array_size; j++) {
		printf("%d ", target_arr[j]);
	}	
	printf("\n");

	return 0;
}

void merge_sort(int l, int h, int *target, int *source) {
	if(l < h) {
		int mid = (l+h) / 2; 
		merge_sort(l, mid, target, source); 
		merge_sort(mid+1, h, target, source); 
		merge(l, mid, h, target, source); 
	}

}

void merge(int lower, int middle, int higher, int *target_arr, int *source_arr) {
	// initialize counters 
	int i = 0; 
	int j = middle+1;
	int k = 0; 
	
	while(i <= lower && j <= higher) {
		if(source_arr[i] < source_arr[j]) {
			target_arr[k] = source_arr[i];
			i++;
			k++; 
		}
		else {
			target_arr[k] = source_arr[j];
			j++;
			k++; 
		}

		// copy over the remaining elements if there are any
		
		for(; i <= lower; i++) {
			target_arr[k] = source_arr[i];
			k++;
		}

		for(; j <= higher; j++) {
			target_arr[k] = source_arr[j];
			k++;
		}
	}


}





