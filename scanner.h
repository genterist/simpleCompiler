/*
 ============================================================================
 Name        : scanner.h
 Author      : NGUYEN, TAM N
 Created on  : 15OCT16
 Version     : 1
 Copyright   : (CC)
 Summary	 : 
 ============================================================================
 */

#ifndef _SCANNER_H_
#define _SCANNER_H_

typedef enum {false, true} bool;
typedef struct Scanner *myScanner; 

#include <stdio.h>

/* Initialize a scanner with default values
*  most important data is the driver table
*/
myScanner initScanner ();


/*Free up memory used by the scanner
*/
void clearScanner(myScanner);


#endif