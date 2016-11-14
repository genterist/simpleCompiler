/*
 ============================================================================
 Name        : parser.h
 Author      : NGUYEN, TAM N
 Created on  : 12NOV16
 Version     : 1
 Copyright   : (CC)
 Summary	 : 
 
 ============================================================================
 */

#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdio.h>
#include "./token.h"
#include "./buildTree.h"
#include "./traversals.h"

//************************
// FUNCTION PROTOTYPES

void parser ( myScanner scanIt, Treeptr myTree);
int launch (int code, myScanner scanIt, Treeptr myTree);

#endif