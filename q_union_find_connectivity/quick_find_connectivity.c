/*
 * =====================================================================================
 *
 *       Filename:  q_union_find_connectivity.c
 *
 *    Description: Quick-find union solution for connectivity problem 
 *
 *        Version:  1.0
 *        Created:  10/8/2024 10:02:07 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */

#include <stdio.h>
#define N 10000

int main(void) {
	
	// p-q pairs - connect p to q
	// i - index counter 
	int i, j, p, q, t, id[N];

	for(i = 0; i<N; ++i) id[i] = i; 
	while(scanf("%d %d\n", &p, &q) == 2) {
		for(i = p; i != id[i]; i = id[i]); 
		for(j = q; j != id[j]; j = id[j]; 
		if(i == j) continue; 
		id[i] = j; 
		printf("%d %d\n", p, q); 
	}
}
