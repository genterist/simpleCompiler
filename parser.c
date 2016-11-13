/*
 ============================================================================
 Name        : parser.c
 Author      : NGUYEN, TAM N
 Created on  : 13NOV16
 Version     : 1
 Copyright   : (CC)
 Summary	 : a simple parser program that uses CFG and top-down recursive descent parsing
                The program will take a file name (file extension of .fs16 is implicit)
                as the first CLI argument or a content stream (either from keyboard or
                file redirection)
                The program will then scan and provide tokens as outputs
                Each found token will then be analyzed and evaluated per CFG.
                
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


//************************
//THE CFG USED
//
//<program>  ->     <vars> <block>
//<block>    ->      Begin <vars> <stats> End
//<vars>     ->      empty | Var Identifier <mvars> 
//<mvars>    ->     empty | : : Identifier <mvars>
//<expr>     ->      <M> + <expr> | <M>
//<M>        ->     <T> - <M> | <T>
//<T>        ->      <F> * <T> | <F> / <T> | <F>
//<F>        ->      - <F> | <R>
//<R>        ->      [ <expr> ] | Identifier | Number   
//<stats>    ->      <stat>  <mStat>
//<mStat>    ->      empty | <stat>  <mStat>
//<stat>     ->      <in> | <out> | <block> | <if> | <loop> | <assign>
//<in>       ->      Scan : Identifier .
//<out>      ->      Print [ <expr>  ] .
//<if>       ->      [ <expr> <RO> <expr> ]  Iff <block>             
//<loop>     ->      Loop [ <expr> <RO> <expr> ] <block>
//<assign>   ->      Identifier == <expr> .                   // == is one token here
//<RO>       ->      >=> | <=< | = |  > | <  |  =!=           // each is one token here


//************************
//PARSER ERROR CODES
//
#define TRUE    1
#define FALSE   0
#define PROGRAM_ERROR       100
#define BLOCK_ERROR         101
#define VARS_ERROR          102
#define MVARS_ERROR         103
#define EXPR_ERROR          104
#define M_ERROR             105
#define T_ERROR             106
#define F_ERROR             107
#define R_ERROR             108
#define STATS_ERROR         109
#define MSTATS_ERROR        110
#define STAT_ERROR          111
#define IN_ERROR            112
#define OUT_ERROR           113
#define IF_ERROR            114
#define LOOP_ERROR          115
#define ASSIGN_ERROR        116
#define RO_ERROR            117
//

//************************
// GLOBAL VARIABLES
myToken t;      // note that t has {int tokenType, char tokenVal[bufLen] , int  tokenLine }


//FUNCTION CODE
#define program_parse_code  200
#define block_parse_code    201
#define vars_parse_code     202
#define stats_parse_code    203
#define mvars_parse_code    204

#define get_next_token      299

// AUX function
int launch (int code, myScanner scanIt) {
    int temp = 0;
    //get a token
    if (hasTokenError (t = getToken(scanIt)) == 0 || strstr(t->tokenVal,"EOF")!=NULL) { // if there is no error
        //call the function
       switch (code) {
            case 200:
                temp = program_parse (scanIt);
                break;
            case 201:
                temp = block_parse (scanIt);
                break;
            case 202:
                temp = vars_parse (scanIt);
                break;
            case 203:
                //temp = stats_parse (scanIt);
                break;
            case 204:
                temp = mvars_parse (scanIt);
            default :
                break;
       }
    } else {
    // display an error if there are issues with getting the token
        fprintf(stderr, "[SYSTEM] Cannot get more information or EOF is reached\n");
    }
    
    return temp;
}
//************************
// FUNCTION IMPLEMENTATIONS

void parser ( myScanner scanIt) {
    
    launch (program_parse_code, scanIt);

    if (t->tokenType == eofCode) {
        fprintf(stderr, "[EOF] Parsing reached the end of file.\n");
    }    
    
    clearToken (t);
    return;
}

//<program>  ->     <vars> <block>
//Return 0 if no <vars> or <block> found
//1 if <var> is found
//2 if <block> is found
int program_parse (myScanner scanIt) {

    if (strstr(t->tokenVal,"Var")!=NULL) {                              // if 'Var' is found
        launch (vars_parse_code, scanIt);                               //call vars_parse
    } 
    if (strstr(t->tokenVal,"Begin")!=NULL) {                            // if 'Begin' is found (no <vars> section)
        launch (block_parse_code, scanIt);
    } else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing main program. \n", t->tokenLine);
        return 0;
    }
    return 1;
}


//<block>    ->      Begin <vars> <stats> End
//return 1 if success, 0 if there's an error
int block_parse (myScanner scanIt) {
    int flag = 1;
    if (strstr(t->tokenVal,"Var")!=NULL) {
        flag = launch(vars_parse_code, scanIt);
    }
    // if vars is all empty, then we check for <stats>
    if (strstr(t->tokenVal,"Scan")!=NULL  || strstr(t->tokenVal,"Print")!=NULL  || 
                strstr(t->tokenVal,"[")!=NULL  || strstr(t->tokenVal,"Loop")!=NULL  || 
                strstr(t->tokenVal,"Begin")!=NULL || t->tokenType==idCode ) {
        flag = launch (stats_parse_code, scanIt);   
    }
    // if found no <stats>, issue syntax error because <stats> can't be empty per given grammar
    else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing the body of the main block. \n", t->tokenLine-1);
        flag = 0;
    }
    
    if (strstr(t->tokenVal,"End")==NULL)
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Program has to be ended with an 'End'\n", t->tokenLine-1);
        flag = 0;
    }
    
    return flag;    
}

//<vars>     ->      empty | Var Identifier <mvars> 
int vars_parse (myScanner scanIt) {
    if (t->tokenType == idCode) {
        launch(mvars_parse_code, scanIt);
    } else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing the body of variable declairation. \n", t->tokenLine-1);
        return 0;
    }
    return 1;
}

//<mvars>    ->     empty | : : Identifier <mvars>
int mvars_parse (myScanner scanIt) {
    int flag = 1;
    if (t->tokenType==otherCode && t->tokenVal[0]==':') {           //reconizing :
        launch (get_next_token, scanIt);  
        if (t->tokenType==otherCode && t->tokenVal[0]==':') {       //recognizing : : , note there is a space in between
            launch (get_next_token, scanIt);
        }  else
        {
            flag = 0;
            fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing another ':' \n", t->tokenLine);
        }
    } else                                                          //if not there
    {
        flag = 0;
        if (t->tokenType!=keywordCode) fprintf(stderr, "[ERROR : line %d] Incorrect syntax of multiple variable declaration. \n", t->tokenLine);
    }
    
    if (t->tokenType == idCode) {
        launch(mvars_parse_code, scanIt);                           // recognizing : : Identifier
    } else
    {
        flag = 0;
        if (t->tokenType!=keywordCode) fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing identifier. \n", t->tokenLine);
    }

    return flag;
}
//<expr>     ->      <M> + <expr> | <M>
//<M>        ->     <T> - <M> | <T>
//<T>        ->      <F> * <T> | <F> / <T> | <F>
//<F>        ->      - <F> | <R>
//<R>        ->      [ <expr> ] | Identifier | Number   
//<stats>    ->      <stat>  <mStat>
int stats_parse (myScanner scanIt) {
    
}
//<mStat>    ->      empty | <stat>  <mStat>
//<stat>     ->      <in> | <out> | <block> | <if> | <loop> | <assign>
//<in>       ->      Scan : Identifier .
//<out>      ->      Print [ <expr>  ] .
//<if>       ->      [ <expr> <RO> <expr> ]  Iff <block>             
//<loop>     ->      Loop [ <expr> <RO> <expr> ] <block>
//<assign>   ->      Identifier == <expr> .                   // == is one token here
//<RO>       ->      >=> | <=< | = |  > | <  |  =!=           // each is one token here
//************************


//************************


//************************