/*
 * =====================================================================================
 *
 *       Filename:  cp_array.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/23/2024 12:56:09 AM
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
#include <string.h>

int main(void) {
	int *target_array = malloc(sizeof(int) * 6);	
	for(int k = 0; k < ((sizeof(int)*6)/4); k++) {
		target_array[k] = k+1; 
	}
	int *source_array = malloc(sizeof(int)*2);
	source_array[0] = 7;
	source_array[1] = 8;

				
       	target_array = (int *)realloc(target_array, (sizeof(target_array)+sizeof(source_array))); 	
	memcpy(target_array+(int)6, source_array, sizeof(source_array));
	
	for(int i = 0; i < 8; i++) {
		printf("Index %d: %d\n", i+1, target_array[i]);  
	}

	return 0;
}

