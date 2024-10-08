/*
 * =====================================================================================
 *
 *       Filename:  quick_find_connectivity.c
 *
 *    Description: Quick-find solution for connectivity problem 
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
	int i, p, q, t, id[N];

	for(i = 0; i<N; ++i) id[i] = i; 
	while(scanf("%d %d\n", &p, &q) == 2) {
		if(id[p] == id[q]) {
			continue;
		}
		for(t = id[p], i = 0; i <N; i++) {
			if(id[i]==t) {
				id[i]  = id[q];
				printf("%d %d\n", p, q); 
			}
		}
	}
}
