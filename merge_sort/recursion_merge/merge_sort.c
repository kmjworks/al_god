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

#define FIRST_INDEX 0

void merge_sort(int l, int h, int *target, int *source);
void merge(int lower, int middle, int higher, int *target_array, int *source_array); 

int main(void) {
	int source_arr[] = { 7,11,3,9,5,6,4,8,2,1 };
	int array_size = sizeof(source_arr) / 4;
	int last_index = array_size - 1;
	int target_arr[array_size];

	printf("Unsorted array: "); 
	for(int i = 0; i < array_size; i++) {
		printf("%d ", source_arr[i]);
	}
	printf("\n");

	merge_sort(FIRST_INDEX, last_index, target_arr, source_arr);

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
		memcpy(source, target, (h+1)*sizeof(int));
	}

}

void merge(int lower, int middle, int higher, int *target_arr, int *source_arr) {
	// initialize counters
	int holder = 0;
	int i = lower;
	int j = middle+1;
	int k = lower; 
	
	while(i <= middle && j <= higher) {
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

		
	}

	// copy over the remaining elements if there are any
	
	for(; i <= middle; i++) {
		target_arr[k] = source_arr[i];	
		k++;
	}

	for(; j <= higher; j++) {
		target_arr[k] = source_arr[j];
		k++; 
	}


}





