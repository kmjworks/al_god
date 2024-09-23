/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/23/2024 12:09:05 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h> 

int merge(int low, int mid, int high, int *s_arr, int *t_arr); 

int main(void) {
	
	int numbers[] = { 1,2,3,4,5,6,7,8 };
	int physical_position_last = sizeof(numbers) / sizeof(int); 
	printf("First index of the array: %d\n", 0);
	printf("Last physical number(not index) of the array: %d\n", physical_position_last);
	printf("Middle part of the array: %d\n", (physical_position_last/2));
	printf("Second middle part of the array: %d\n", 7 / 2); 
	return 0;
}

void merge_sort(int low, int high) { 	
	if(low < high) {
	int middle = (low+high)/2; 
	merge_sort(low, middle);
	merge_sort(middle+1, high-1); 
   	merge(low, middle, high, (int*)0, (int*)1);  	
	}
	// copy the memory from base array to the original array

}


int merge(int low_index, int middle_index, int high_index, int *source_arr, int *target_arr) {
/*
	int i = 0;
 	int j = 0;
	int k = 0;

	if(i <= middle_index && j <= high_index) {
		if(arr[i] < arr[j]) 
			target_array[k] = arr[i];
			i++; 
			k++;
		else 
			target_array[k] = arr[j]; 
			j++; 
			k++;
		
	}	
 */


	// memcpy(target_arr+low_index, source_arr, sizeof(int)*(highest+1) )

	return 0;
}







