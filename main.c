/*
 ============================================================================
 Name        : main.c
 Author      : NGUYEN, TAM N
 Created on  : 15OCT16
 Version     : 1
 Copyright   : (CC)
 Summary	 : a simple token scanner that use automaton driver table method
                The program will take a file name (file extension of .fs16 is implicit)
                as the first CLI argument or a content stream (either from keyboard or
                file redirection)
                The program will then scan and provide tokens as outputs
                Each found token will be printed out on each individual line, with token
                name, token value, and the line where the token is found
                The program will stop when there is an error with lexical syntax, invalid
                characters found or error with content stream.
 ============================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

#include "./scanner.h"
#include "./token.h"
#include "./parser.h"
#include "./buildTree.h"
#include "./traversals.h"


int main(int argc, char *argv[])
{
    myScanner scanIt;                      // init scanner
                                           //init tree

	//myBNTree = NULL;
    
	char *prog = argv[0];                  // capture program name here

	// if user put in many arguments
	if (argc>2) {
		fprintf (stderr,"ERROR: Too many arguments !! \n");
		fprintf (stderr,"       %s [data file name] \n", prog);
	} else if (argv[1]!=NULL) 
	    { scanIt = scanByName(argv[1]);}   // if file name specified, read from file
	  else
	    { scanIt = scanByStream(stdin);}   // if file name not specified, read from input stream
	
    parser(scanIt);
    TravPreOrder(theTree, 0, "parseResult");
	clearScanner (scanIt);
	
	return EXIT_SUCCESS;
}
