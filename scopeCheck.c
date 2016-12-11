/*
 ============================================================================
 Name        : scopeCheck.c
 Author      : NGUYEN, TAM N
 Created on  : 11DEC16
 Version     : 1
 Copyright   : (CC)
 Summary	 : checking scopes of blocks and variables in a successfully built BSTree
 Functions	 :
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "./scopeCheck.h"

//note that we have these arrays
int scopes[100] = {0};
int scopeMembers[100] = {0};


int length = sizeof(scopeMembers) / sizeof(int);

int scope_get_begin_index (int scope) {
    int temp=0;
    if (scope==0) temp = 0;
    else {    
       while (scope>=0){
         scope--;
         temp += scopes[scope];
       }
           //temp--;
    }
    
    return temp;
}

int scope_get_end_index (int scope) {
    int temp;
    if (scopes[scope]==0) temp = 0;
    else {
        temp = scope_get_begin_index(scope) + scopes[scope];
    }

    return temp;
}

int scope_add (int scope, int val) {
    int temp=length;
    int posTail = scope_get_end_index(scope);
    int posStart = scope_get_begin_index(scope);
    //printf("posTail: %d \n", posTail);
    while (temp>posStart) {
        // move all the back members one position away, to the right
        scopeMembers[temp] = scopeMembers[temp-1];
        temp--;
    }
    scopes[scope]++;
    //printf("Scope %d members: %d \n", scope, scopes[scope]);
    //printf("posStart: %d \n", posStart);
    scopeMembers[posStart]=val;
    //posTail = scope_get_end_index(scope);
    //printf("new posTail: %d \n", posTail);
    
    return 1;
}

void scope_print () {
    int ctr = 0;
    int begin, end;
    
    for (ctr; ctr <=100; ctr++)
    {
        if (scopes[ctr]>0){
            begin = scope_get_begin_index(ctr);
            //printf("posStart: %d \n", begin);
            end = scope_get_end_index(ctr);
            //printf("posTail: %d \n", end);
            printf(" | ");
            for (begin; begin<end; begin++){
                printf("%d ", scopeMembers[begin]);
            }
        }

    }
    printf("\n");
    return;
}

