/*
 * =====================================================================================
 *
 *       Filename:  merge_sort.c
 *
 *    Description: Merge sort algorithm 
 *
 *        Version:  1.0
 *        Created:  09/22/2024 08:56:27 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  NULL, 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int* merge_sort(int *n_arr, int f_arr_length, int *target_arr);

int main(void) {

	int numbers[] = { 4,5,6,8,1,2,3,7 };
	int sorted_numbers[32];
	merge_sort(numbers, 4, sorted_numbers);
	printf("Sorted list: \n");
	for(int i = 0; i < 8; i++) {
		printf("%d\n", sorted_numbers[i]);
	}
	return 0;
}


int* merge_sort(int *n_arr, int first_arr_length, int *target_arr) {
	int sorted_array[32];
	int k = 0; 
	int i = 0; 
	int j = 4; // This should equal 4; 
	while(i <= first_arr_length && j <= 8) {
		if(n_arr[i] < n_arr[j]) {
			sorted_array[k] =  n_arr[i];
			i++;
			k++;
		} else {
			sorted_array[k] = n_arr[j];
			j++;
			k++;
		}		
	}	
	 
	for(;i <= first_arr_length; i++) {
		sorted_array[k] = n_arr[i];
		k++;
	}

	for(; j <= ((sizeof(n_arr) / 4));j++) {
		sorted_array[k] = n_arr[j];
		k++;
	}

	memcpy(target_arr, sorted_array, 32);
	return 0;	

}


