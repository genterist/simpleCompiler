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

//************************
// GLOBAL VARIABLES



//************************
// FUNCTION IMPLEMENTATIONS

void parser ( myScanner scanIt) {
    myToken t;
    while (hasTokenError (t = getToken(scanIt)) == 0) {       // if there is any error returned in token value
	    printToken (t);
    }

	
    //printDriverTable (scanIt);
    clearToken (t);
}

//************************


//************************


//************************