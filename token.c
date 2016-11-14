/*
 ============================================================================
 Name        : token.c
 Author      : NGUYEN, TAM N
 Created on  : 15OCT16
 Version     : 1
 Copyright   : (CC)
 Summary	 : 
 Functions	 :  myToken initToken () 
                void clearToken 
                int findNextState (const int currentState,const char c, int stateTable[100][100])
                myToken checkReservedWord(myToken t)
                myToken getToken(myScanner s) 
                int hasTokenError (myToken t) 
                void printToken (myToken t) 
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./token.h"

//configurations
//int bufLen = 100;
int line = 0;
int lineFlag = 0;


/*****************
 * Function:
 *      initToken
 * Description: 
 *      initialize a token ADT, allocating memory needed
 * Input:
 *      none
 * Output:
 *      a token ADT
 *****************/
myToken initToken () {
    
    myToken    t;
    // allocate memory for scanner
	t = malloc(sizeof(struct Token));
	t->tokenLine = 0;
    
    return t;
}


/*****************
 * Function:
 *      clearToken
 * Description: 
 *      remove a token ADT from memory
 * Input:
 *      an initialized token ADT
 * Output:
 *      freed up memory previously occupied by the token ADT
 *****************/
void clearToken (myToken t)
{
	// de-allocate memory
	if (t != NULL) {
		free(t);
	}
}


/*****************
 * Function:
 *      findNextState
 * Description:
 *      This function is internal to the ADT. 
 *      The function will translate a character to its respective ascii value
 *      The initial state is 0 (when no character is read)
 *      Based on the translated value, the function will find the next state
 *      per driver table specified by the scanner
 * Input:
 *      current driver state, a character and a reference to a scanner's driver table
 * Output:
 *      an integer value (of the next state)
 *****************/
int findNextState (const int currentState,const char c, int stateTable[100][100]) {
    int nextState = 0;
    int currentVal = (int) c;
    //map currentVal
    if ((currentVal>=65 && currentVal<=90) || (currentVal>=97 && currentVal<=122)){
        currentVal = 0; //map to col 0 in mapped automaton table
    }
    else if (currentVal>=48 && currentVal <=57){
        currentVal = 1;
    }
    else if (currentVal == 32) currentVal = 2;
    else if (currentVal == 61) currentVal = 3;
    else if (currentVal == 60) currentVal = 4;
    else if (currentVal == 62) currentVal = 5;
    else if (currentVal == 33) currentVal = 6;
    else if (currentVal == 58) currentVal = 7;
    else if (currentVal == 43) currentVal = 8;
    else if (currentVal == 45) currentVal = 9;
    else if (currentVal == 42) currentVal = 10;
    else if (currentVal == 47) currentVal = 11;
    else if (currentVal == 38) currentVal = 12;
    else if (currentVal == 37) currentVal = 13;
    else if (currentVal == 46) currentVal = 14;
    else if (currentVal == 40) currentVal = 15;
    else if (currentVal == 41) currentVal = 16;
    else if (currentVal == 44) currentVal = 17;
    else if (currentVal == 123) currentVal = 18;
    else if (currentVal == 125) currentVal = 19;
    else if (currentVal == 59) currentVal = 20;
    else if (currentVal == 91) currentVal = 21;
    else if (currentVal == 93) currentVal = 22;
    else if (currentVal == 64) currentVal = 23;
    else currentVal = 24;
    
    nextState = stateTable[currentState][currentVal];
    //printf ("Next state is : %d \n", nextState);
    return nextState;
    
}

/*****************
 * Function:
 *      checkReservedWord
 * Description: 
 *      check if a token is indeed a keyword token
 * Input:
 *      a token ADT
 * Output:
 *      a token ADT
 *****************/
myToken checkReservedWord(myToken t) {
    if ((strstr(t->tokenVal,"Begin")!=NULL ||
        strstr(t->tokenVal,"End")!=NULL ||
        strstr(t->tokenVal,"Start")!=NULL ||
        strstr(t->tokenVal,"Stop")!=NULL ||
        strstr(t->tokenVal,"Iff")!=NULL ||
        strstr(t->tokenVal,"Loop")!=NULL ||
        strstr(t->tokenVal,"Void")!=NULL ||
        strstr(t->tokenVal,"Var")!=NULL ||
        strstr(t->tokenVal,"Int")!=NULL ||
        strstr(t->tokenVal,"Call")!=NULL ||
        strstr(t->tokenVal,"Return")!=NULL ||
        strstr(t->tokenVal,"Scan")!=NULL ||
        strstr(t->tokenVal,"Print")!=NULL ||
        strstr(t->tokenVal,"Program")!=NULL) &&
        strstr(t->tokenVal,"@")==NULL               // rule out the chance of keywords appear in a comment
    )

        t->tokenType = keywordCode;
    
    return t;
}


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
myToken getToken(myScanner s) {
    
    int currentState = 0;
    int charRead = 0;
    char c;
    char buffer[bufLen];
    int flag = 0;
    int eofFlag = 0;
    
    // initialize token
    myToken t;
    t = initToken ();
    // get first character from the scanner
    c = fgetc(s->fp);
    
    // now process the received character
    while (flag==0) {
        
        if (charRead + 1 > bufLen) {
            flag = 1;
            fprintf (stderr,"ERROR: Token name exceeds %d characters!! \n", bufLen);
            }                     //concat all characters beyond buffer
        if (c == '\n') {                                        //in case of a new line 
            lineFlag++;
            c = ' ';                                            //treat it as a white space
        }
        
        if (c == EOF) {
            //fprintf (stderr,"End of file reached. \n");
            c = ' ';                                           //treat it as a white space
            flag = 1;                                          
            eofFlag = 1;
        }
        
		currentState = findNextState (currentState, c, s->table);
		
		if (currentState>=991 && currentState <=999) {            // if a correct token is found
		    flag = 1;
		    line = line + lineFlag;
            lineFlag = 0;
		    t->tokenType = currentState;
		    strcpy(t->tokenVal, buffer);
		    t->tokenLine = line;
		    memset(&buffer[0], 0, sizeof(buffer));                // clearing the buffer
            ungetc(c, s->fp);                                     // step back one character
		}
		else if (currentState>=980 && currentState <=989) {       // if an error is found
            if (currentState==980) fprintf (stderr,"ERROR : Invalid token!! \n");
            if (currentState==981) fprintf (stderr,"ERROR : Character is not in the allowed alphabet. \n");
            flag = 1;
		    t->tokenType = currentState;
		    strcpy(t->tokenVal, "[Error]");
		    t->tokenLine = line;
		    //memset(&buffer[0], 0, sizeof(buffer));                // clearing the buffer
        }
        else {
            int temp;
            temp = isspace((int) c);
            if (temp == 0) buffer[charRead++] = c;							      // append c to buffer, skip white space
            c = fgetc(s->fp);
            
        }

	}
    
    t = checkReservedWord(t);

    if (eofFlag == 1) {
        strcpy(t->tokenVal, "EOF");
        t->tokenType = eofCode; //998
    }

    
    return t;
}


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
int hasTokenError (myToken t) {
    if (strcmp(t->tokenVal, "[Error]") == 0) return 1;
    if (strcmp(t->tokenVal, "EOF") == 0) return 1;
    if (strcmp(t->tokenVal, "") == 0) return 1;
    return 0;
}


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
void printToken (myToken t) {
    char tokenName[50];
    int x = t->tokenType;
    if (x==idCode) strcpy(tokenName,"Identifier");
    else if (x==relCode) strcpy(tokenName,"Relational operator");
    else if (x==otherCode) strcpy(tokenName,"Other operator");
    else if (x==delimCode) strcpy(tokenName,"Delimiter");
    else if (x==intCode) strcpy(tokenName,"Integer");
    else if (x==comCode) strcpy(tokenName,"Comment");
    else if (x==keywordCode) strcpy(tokenName,"Keyword");
    else strcpy(tokenName,"[ERROR]");
    
    printf("\{ %s , %s , %d }\n",tokenName, t->tokenVal, t->tokenLine);
    memset(&tokenName[0], 0, sizeof(tokenName)); 
}

