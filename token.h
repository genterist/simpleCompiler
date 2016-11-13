/*
 ============================================================================
 Name        : scanner.h
 Author      : NGUYEN, TAM N
 Created on  : 15OCT16
 Version     : 1
 Copyright   : (CC)
 Summary	 : A basic token parser based on a predefined driver table declared
                in the scanner ADT, and a predefined sets of token types.
                Each character received from the scanner will be evaluated and 
                the process stops when a complete token is found or an error is
                encountered.
                Character processing is based on US Ascii table so this program
                may not work correctly with Unicode characters. New line is
                processed as "\n" so this program may not work well with systems
                that use a different new line marking.
                The codes and inner workings are mostly based on integers, and
                ranges of integers are used in logic checking. Current range of
                allowed tokens is from 990 to 999, and range of allowed error code
                is from 980 to 989.
                
 ============================================================================
 */

#ifndef _TOKEN_H_
#define _TOKEN_H_


#include <stdio.h>
#include "./scanner.h"

#define bufLen (100)

//configuration : error codes
#define invalidToken_errorCode (980)
#define invalidCharacter_errorCode (981)

//configuration : token codes
#define idCode (991)
#define relCode (992)
#define otherCode (993)
#define delimCode (994)
#define intCode (995)
#define comCode (996)
#define keywordCode (997)
#define eofCode (998)

//declare Token structure
typedef struct Token *myToken;
struct Token {
    int tokenType;
    char tokenVal[bufLen];
    int  tokenLine;
};

/*****************
 * Function:
 *      printToken
 * Description: 
 *      This function is globally available
 *      It prints descriptive information about a token to the screen
 *      which includes token type, token value and the line where
 *      the token is found
 * Input:
 *      a token ADT
 * Output:
 *      A descriptive printout of a particular token to the screen
 *****************/
void printToken (myToken t);


/*****************
 * Function:
 *      getToken
 * Description: 
 *      This function is globally available
 *      The function will process each character received from the scanner
 *      and try to construct a complete token by using the scanner driver.
 *      The process stops when a specified token code is reached or an error
 *      code is encountered.
 * Input:
 *      a scanner ADT
 * Output:
 *      a token ADT
 *****************/
myToken getToken(myScanner s);


/*****************
 * Function:
 *      hasTokenError
 * Description: 
 *      This function is availble globally
 *      This function double checks if there is
 *      a problem with a particular token.
 *      This is used to stop the while loop of main program
 *      alling scanner and getToken function.
 * Input:
 *      a toke ADT
 * Output:
 *      0 if there is problem
 *      1 if there is a problem
 *****************/
int hasTokenError (myToken t); 

#endif