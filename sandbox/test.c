#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data; 
    struct Node *next; 

}; 

void populate_nodes(int items_n, int items[]); 

int count_nodes(struct Node* f_ptr); 
struct Node *first_n; 
int main(void) 
{

    int data_list[] = { 1,56,34,2,6,7 };    
    populate_nodes(6, data_list);
    printf("Number of nodes : %d\n", count_nodes(first_n)); 
    return 0; 
}

struct Node* getfirst_node(struct Node *ptr) {
    return ptr; 
}

void populate_nodes(int items_n, int items[]) {
    struct Node *t, *last;
    t = (struct Node*)malloc(sizeof(struct Node));
    last = (struct Node*)malloc(sizeof(struct Node)); 
    t->data = items[0]; 
    t->next = NULL; 
    last = t;  

    for(int i = 1; i < items_n; i++) {
        printf("%d\n", t->data); 
        t->next = (struct Node*)malloc(sizeof(struct Node*));
        if(i == 1) {
            first_n = getfirst_node(t); 
        }
        t = t->next; 
        t->data = items[i]; 
        last = t; 
    }
}


int count_nodes(struct Node* f_ptr) {
    if(f_ptr == 0) {
        return 0; 
    }


    return count_nodes(f_ptr->next)+1; 
}
