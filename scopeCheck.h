/*
 ============================================================================
 Name        : scopeCheck.h
 Author      : NGUYEN, TAM N
 Created on  : 11DEC16
 Version     : 1
 Copyright   : (CC)
 Summary	 : checking scopes of blocks and variables in a successfully built BSTree
 Functions	 :
 ============================================================================
 */

#ifndef SCOPECHECK_H_
#define SCOPECHECK_H_

#include <stdio.h>
#include <stdlib.h>

extern int scopes[100];
extern int scopeMembers[100];

int scope_add (int scope, int val);
void scope_print ();

#endif /* SCOPECHECK_H_ */