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

//************************
// FUNCTION PROTOTYPES

void parser ( myScanner scanIt);
int launch (int a, myScanner scanIt);

#endif