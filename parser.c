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
#include "./buildTree.h"
#include "./traversals.h"
#include "./scopeCheck.h"

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
// GLOBAL VARIABLES
myToken t;      // note that t has {int tokenType, char tokenVal[bufLen] , int  tokenLine }


//FUNCTION CODE
#define program_parse_code  200
#define block_parse_code    201
#define vars_parse_code     202
#define stats_parse_code    203
#define mvars_parse_code    204
#define scan_parse_code     205
#define print_parse_code    206
#define iff_parse_code      207
#define loop_parse_code     208
#define assign_parse_code   209
#define get_next_token      299

//FUNCTION PROTOTYPES
void parser ( myScanner scanIt, Treeptr aTree );
int program_parse (myScanner scanIt, Treeptr aTree );
int block_parse (myScanner scanIt, Treeptr parentNode );
int vars_parse (myScanner scanIt,  Treeptr parentNode );
int stats_parse (myScanner scanIt,  Treeptr parentNode );
int mvars_parse (myScanner scanIt,  Treeptr parentNode );
int scan_parse (myScanner scanIt,  Treeptr parentNode );
int print_parse (myScanner scanIt,  Treeptr parentNode );
int iff_parse (myScanner scanIt,  Treeptr parentNode );
int loop_parse (myScanner scanIt,  Treeptr parentNode );
int assign_parse (myScanner scanIt,  Treeptr parentNode );
int stat_parse (myScanner scanIt,  Treeptr parentNode );
int mStat_parse (myScanner scanIt,  Treeptr parentNode );
int exprs_parse(myScanner scanIt,  Treeptr parentNode );
int expr_parse(myScanner scanIt,  Treeptr parentNode );
int M_parse(myScanner scanIt,  Treeptr parentNode );
int T_parse(myScanner scanIt,  Treeptr parentNode );
int F_parse(myScanner scanIt,  Treeptr parentNode );
int R_parse(myScanner scanIt,  Treeptr parentNode );


// AUX function
int launch (int code, myScanner scanIt, Treeptr parentNode ) {
    int temp = 0;
    //consume the matched token that calls the corresponding function
    //get the next token
    if (hasTokenError (t = getToken(scanIt)) == 0 || strstr(t->tokenVal,"EOF")!=NULL) { // if there is no error
        //call the function, feel free to enable, disable or inject troubleshooting routines to these switches
       switch (code) {
            case 200:
                temp = program_parse (scanIt, parentNode);
                break;
            case 201:
                temp = block_parse (scanIt, parentNode);
                break;
            case 202:
                temp = vars_parse (scanIt, parentNode);
                break;
            case 203:
                temp = stats_parse (scanIt, parentNode);
                break;
            case 204:
                temp = mvars_parse (scanIt, parentNode);
                break;
            case 205:
                temp = scan_parse (scanIt, parentNode);
                break;
            case 206: 
                temp = print_parse (scanIt, parentNode);
                break;
            case 207:
                temp = iff_parse (scanIt, parentNode);
                break;
            case 208:
                temp = loop_parse (scanIt, parentNode);
                break;
            case 209:
                temp = assign_parse (scanIt, parentNode);
                break;
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

void parser ( myScanner scanIt, Treeptr aTree ) {
    
    launch (program_parse_code, scanIt, aTree);

    if (t->tokenType == eofCode) {
        fprintf(stderr, "[EOF] Parsing reached the end of file.\n");
    }    
    
    clearToken (t);
    return;
}

//<program>  ->     <vars> <block>
//Return 0 if no <vars> or <block> found
int program_parse (myScanner scanIt, Treeptr aTree ) {
    fprintf(stderr, "parsing ...\n");
    aTree= buildTree("<program>","", aTree);
    Treeptr varNode, blockNode;
    
    int flag = 1;
    if (strstr(t->tokenVal,"Var")!=NULL) {                              // if 'Var' is found
        varNode = buildTree( "<vars>", "", aTree);
        if (launch (vars_parse_code, scanIt, varNode)==0) flag--;                //call vars_parse
    } 
    if (strstr(t->tokenVal,"Begin")!=NULL) {                            // if 'Begin' is found (no <vars> section)
        blockNode = buildTree("<block>", "", aTree);
        if (launch (block_parse_code, scanIt, blockNode)==0) flag--;
    } else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing main program. \n", t->tokenLine);
        flag--;
    }
    if (flag <=0) return 0;
    else 
        return 1;
}


//<block>    ->      Begin <vars> <stats> End
//return 1 if success, 0 if there's an error
int block_parse (myScanner scanIt, Treeptr parentNode ) {
    Treeptr varsNode,statsNode;
    int flag = 1;
    if (strstr(t->tokenVal,"Var")!=NULL) {
        varsNode = buildTree("<Vars>", "", parentNode);
        if ( launch(vars_parse_code, scanIt, varsNode)==0 ) flag--;
    }
    // if vars is all empty, then we check for <stats>
    //<stats>    ->      <stat>  <mStat>
    // since <stat> is mandatory, in this step, we check members of <stat> 
    //and call function accordingly, note that we check qualifying condition for a function
    //before we call the function

    statsNode = buildTree("<Stats>", "", parentNode);
    if (stats_parse (scanIt, statsNode)==0) flag--;                            //check for <stats>
    
    if (strstr(t->tokenVal,"End")==NULL)
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Program has to be ended with an 'End'\n", t->tokenLine-1);
        flag --;
    }
    
    if (flag <= 0) return 0;
    else
        return 1;
}

//<vars>     ->      empty | Var Identifier <mvars> 
int vars_parse (myScanner scanIt, Treeptr parentNode ) {
    Treeptr idNode;
    int flag = 1;
    if (t->tokenType == idCode) {
        idNode = buildTree( "<ID>", t->tokenVal, parentNode);
        launch(mvars_parse_code, scanIt, parentNode);            // check for <mvars>
    } else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing the body of variable declairation. \n", t->tokenLine-1);
        flag--;
    }
    if (flag <= 0) return 0;
    else
        return 1;
}

//<mvars>    ->     empty | : : Identifier <mvars>
int mvars_parse (myScanner scanIt, Treeptr parentNode ) {
    Treeptr mvarsNode, idNode;
    int flag = 1;
    if (t->tokenType == idCode) {                                   //if an identifier is found
        mvarsNode = buildTree( "<mvars>", "", parentNode);
        idNode = buildTree( "<ID>", t->tokenVal, mvarsNode);
        launch(mvars_parse_code, scanIt, mvarsNode);                // recognizing : : Identifier
    }else if (t->tokenType==otherCode && t->tokenVal[0]==':') {     //reconizing :
        if (launch (get_next_token, scanIt, parentNode)==0) flag--; //need to check the passing of tempNode here
        if (t->tokenType==otherCode && t->tokenVal[0]==':') {       //recognizing : : , note there is a space in between
            if (launch (get_next_token, scanIt, parentNode)==0) flag--;
            mvars_parse (scanIt, parentNode);
        }  else
        {
            flag--;
            fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing another ':' \n", t->tokenLine);
        }
    } else                                                          //if not there
    {
        flag--;
        if (t->tokenType!=keywordCode) fprintf(stderr, "[ERROR : line %d] Incorrect syntax of multiple variable declaration. \n", t->tokenLine);
    }

    if (flag <= 0) return 0;
    else
        return 1;
}

//<stats>    ->      <stat>  <mStat>
int stats_parse (myScanner scanIt, Treeptr parentNode ) {
    int flag = 1;
    Treeptr statNode, mstatNode;
    statNode = buildTree("<stat>", "", parentNode);
    if (stat_parse (scanIt, statNode)<=0) flag--;                                             // check for <stat>
    //else {
        mstatNode = buildTree("<mStat>", "", parentNode);
        mStat_parse (scanIt, mstatNode);                                       // check for <mStat>
    //}    

    //note that launch function is not used in these function calls
    //no current token got consumed yet
    if (flag <= 0) return 0;
    else
        return 1;
}

//<mStat>    ->      empty | <stat>  <mStat>
int mStat_parse (myScanner scanIt, Treeptr parentNode ) {
    int flag = 1;
    Treeptr statNode;
    statNode = buildTree("<stat>","", parentNode);
    if (stat_parse(scanIt, statNode)==1) {
        if (mStat_parse (scanIt, parentNode)==0) flag--;
    }
    else
    {
        flag--;
    }

    return flag;
}

//<stat>     ->      <in> | <out> | <block> | <if> | <loop> | <assign>
int stat_parse (myScanner scanIt, Treeptr parentNode ) {
    Treeptr tempNode;
    int flag = 0;
    if (strstr(t->tokenVal,"Scan")!=NULL) {
        flag++;
        tempNode = buildTree( "<In>","", parentNode);
        if(launch (scan_parse_code, scanIt, tempNode)<=0) flag--;
    }
    else if (strstr(t->tokenVal,"Print")!=NULL) {
        flag++;
        tempNode = buildTree( "<Out>","", parentNode);
        if(launch (print_parse_code, scanIt, tempNode)<=0) flag--;
    }  
    else if (strstr(t->tokenVal,"[")!=NULL) {
        flag++;
        tempNode = buildTree( "<If>","", parentNode);
        if(launch (iff_parse_code, scanIt, tempNode)<=0) flag--;
    }
    else if (strstr(t->tokenVal,"Loop")!=NULL) {
        flag++;
        tempNode = buildTree( "<Loop>","", parentNode);
        if(launch (loop_parse_code, scanIt, tempNode)<=0) flag--;
    }
    else if (strstr(t->tokenVal,"Begin")!=NULL) {
        flag++;
        tempNode = buildTree( "<block>","", parentNode);
        if(launch (block_parse_code, scanIt, tempNode)<=0) flag--;
    }
    else if (t->tokenType==idCode ) {
        flag++;
        Treeptr assignNode;
        assignNode = buildTree( "<assign>","", parentNode);
        tempNode = buildTree( "<ID>", t->tokenVal, assignNode);
        if(launch (assign_parse_code, scanIt, assignNode)<=0) flag--;
    }
    else if (strstr(t->tokenVal,"End")!=NULL) {
        if(launch (get_next_token, scanIt, tempNode)==0) flag--;
    }
    // if found no <stats>, issue syntax error because <stats> can't be empty per given grammar
    else if (flag < 0)
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing the body of <stat> block. \n", t->tokenLine-1);
    }
    return flag;
    
}
//<in>       ->      Scan : Identifier .
int scan_parse (myScanner scanIt, Treeptr parentNode ) {
    Treeptr tempNode;
    int flag = 1;
    
    if (strstr(t->tokenVal,":")!=NULL) {
        if(launch (get_next_token, scanIt, tempNode)==0) flag--;
    }
    else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing ':'. \n", t->tokenLine-1);
        flag --;
    }

    if (t->tokenType==idCode ) {
        tempNode = buildTree( "<ID>", t->tokenVal, parentNode);
        if(launch (get_next_token, scanIt, tempNode)==0) flag--;
    }
    else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing identifier. \n", t->tokenLine-1);
        flag --;
    }
    
    if (strstr(t->tokenVal,".")!=NULL) {
        if(launch (get_next_token, scanIt, tempNode)==0) flag--;
    }
    else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing '.' \n", t->tokenLine-1);
        flag --;
    }
    
    if (flag <= 0) return 0;
    else
        return 1;
}
//<out>      ->      Print [ <expr>  ] .
int print_parse (myScanner scanIt, Treeptr parentNode ) {
    Treeptr tempNode;
    int flag = 1;
    if (strstr(t->tokenVal,"[")!=NULL) {
        if(launch (get_next_token, scanIt, tempNode)==0) flag--;
    }
    else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax in 'Print' function. Missing '['. \n", t->tokenLine-1);
        flag --;
    }
    
    if (strstr(t->tokenVal,"-")!=NULL || strstr(t->tokenVal,"[")!=NULL || t->tokenType==idCode || t->tokenType==intCode) {
        tempNode = buildTree( "<Expr>", "",parentNode);
        if(expr_parse(scanIt, tempNode)==0) flag--;
    }
    else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax in 'Print' command. Missing <expr>. \n", t->tokenLine-1);
        flag --;
    }
    
    if (strstr(t->tokenVal,"]")!=NULL) {
        if(launch (get_next_token, scanIt, tempNode)==0) flag--;
    }
    else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax in 'Print' command. Missing ']'. \n", t->tokenLine-1);
        flag --;
    }
    
    if (strstr(t->tokenVal,".")!=NULL) {
        if(launch (get_next_token, scanIt, tempNode)==0) flag--;
    }
    else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax in 'Print' command. Missing '.' \n", t->tokenLine-1);
        flag --;
    }
    if (flag <= 0) return 0;
    else
        return 1;
}

//<if>       ->      [ <expr> <RO> <expr> ]  Iff <block> 
int iff_parse (myScanner scanIt, Treeptr parentNode ) {
    Treeptr roNode,blockNode, tempNode;
    roNode = buildTree( "<RO>","", parentNode);
    blockNode = buildTree( "<block>","", parentNode);
    int flag = 1;
    
                                                            //check for <expr>
    if (t->tokenVal[0]=="-" || t->tokenVal[0] =="[" || t->tokenType==idCode || t->tokenType==intCode) {
        tempNode = buildTree( "<expr>","", roNode);
        if(expr_parse(scanIt, tempNode)==0) flag--;
    }
    else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing <expr>. \n", t->tokenLine-1);
        flag --;
    }
                                                            //check for <RO>    
    if (t->tokenType==relCode ) {
        // need to record RO data here (greater, equal or something)
        strcpy(roNode->value,t->tokenVal);
        if(launch (get_next_token, scanIt, tempNode)==0) flag--;
    }
    else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing relational symbol. \n", t->tokenLine-1);
        flag --;
    }   
                                                            //check for another <expr>
    if (strstr(t->tokenVal,"-")!=NULL || strstr(t->tokenVal,"[")!=NULL || t->tokenType==idCode || t->tokenType==intCode) {
        tempNode = buildTree( "<expr>","", roNode);
        if(expr_parse(scanIt, tempNode)==0) flag--;
    }
    else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing <expr>. \n", t->tokenLine-1);
        flag --;
    }
                                                            //check for ']'
    if (strstr(t->tokenVal,"]")!=NULL) {
        if(launch (get_next_token, scanIt, tempNode)==0) flag--;
    }
    else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing ']'. \n", t->tokenLine-1);
        flag --;
    }
                                                            //check for Iff
    if (strstr(t->tokenVal,"Iff")!=NULL) {
        if(launch (get_next_token, scanIt, tempNode)==0) flag--;
    }
    else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing 'Iff'. \n", t->tokenLine-1);
        flag --;
    }
    if (strstr(t->tokenVal,"Begin")!=NULL) {                            // if 'Begin' is found (no <vars> section)
        if (launch (block_parse_code, scanIt, blockNode)==0) flag--;
    } else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing <block>. \n", t->tokenLine);
        flag--;
    }
    if (flag <= 0) return 0;
    else
        return 1;
}            
//<loop>     ->      Loop [ <expr> <RO> <expr> ] <block>
int loop_parse (myScanner scanIt, Treeptr parentNode ) {
    Treeptr roNode,blockNode, tempNode;
    roNode = buildTree( "<RO>","", parentNode);
    blockNode = buildTree( "<block>","", parentNode);
    int flag = 1;
    
                                                            //check for <expr>
    if (strstr(t->tokenVal,"-")!=NULL || strstr(t->tokenVal,"[")!=NULL || t->tokenType==idCode || t->tokenType==intCode) {
        tempNode = buildTree( "<expr>","", roNode);
        if(expr_parse(scanIt, tempNode)==0) flag--;
    }
    else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing <expr>. \n", t->tokenLine-1);
        flag --;
    }
                                                            //check for <RO>    
    if (t->tokenType==relCode ) {
        //put relational codes here
        strcpy(roNode->value,t->tokenVal);
        if(launch (get_next_token, scanIt, tempNode)==0) flag--;
    }
    else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing relational symbol. \n", t->tokenLine-1);
        flag --;
    }   
                                                            //check for another <expr>
    if (strstr(t->tokenVal,"-")!=NULL || strstr(t->tokenVal,"[")!=NULL || t->tokenType==idCode || t->tokenType==intCode) {
        tempNode = buildTree( "<expr>","", roNode);
        if(expr_parse(scanIt, tempNode)==0) flag--;
    }
    else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing <expr>. \n", t->tokenLine-1);
        flag --;
    }
                                                            //check for ']'
    if (strstr(t->tokenVal,"]")!=NULL) {
        if(launch (get_next_token, scanIt, tempNode)==0) flag--;
    }
    else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing ']'. \n", t->tokenLine-1);
        flag --;
    }
    if (strstr(t->tokenVal,"Begin")!=NULL) {                  // if 'Begin' is found (no <vars> section)
        if (launch (block_parse_code, scanIt, blockNode)==0) flag--;
    } else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing <block>. \n", t->tokenLine);
        flag--;
    }
    if (flag <= 0) return 0;
    else
        return 1;
}

//<assign>   ->      Identifier == <expr> .                   // == is one token here
int assign_parse (myScanner scanIt, Treeptr parentNode ) {
    Treeptr tempNode;
    int flag = 1;
    if (strstr(t->tokenVal,"==")!=NULL) {
        strcpy(parentNode->value,t->tokenVal);
        if(launch (get_next_token, scanIt, tempNode)==0) flag--;
    }
    else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing '=='. \n", t->tokenLine-1);
        flag --;
    }
    
    if (strstr(t->tokenVal,"-")!=NULL || strstr(t->tokenVal,"[")!=NULL || t->tokenType==idCode || t->tokenType==intCode) {
        tempNode = buildTree( "<expr>","", parentNode);
        if(expr_parse(scanIt, tempNode)==0) flag--;
    }
    else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing <expr>. \n", t->tokenLine-1);
        flag --;
    }
    
    if (strstr(t->tokenVal,".")!=NULL) {
        if(launch (get_next_token, scanIt, tempNode)==0) flag--;
    }
    else
    {
        fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing '.' \n", t->tokenLine-1);
        flag --;
    }
    if (flag <= 0) return 0;
    else
        return 1;  
}

//<expr>     ->      <M> + <expr> | <M>
//expr is a wrapper
int expr_parse(myScanner scanIt, Treeptr parentNode ) {
    Treeptr tempNode, mNode;
    int flag = 1;
    mNode = buildTree( "<M>","", parentNode);
    if(M_parse(scanIt, mNode)==1){
        if(launch (get_next_token, scanIt, parentNode)==0) flag--;
    }
    else flag--;
    
    if (strstr(t->tokenVal,"+")!=NULL) {
        strcpy(parentNode->value,t->tokenVal);
        if(launch (get_next_token, scanIt, tempNode)==0) flag--;
        tempNode = buildTree( "<expr>","", parentNode);
        if(expr_parse(scanIt, tempNode)==0) flag--;
    }
    
    if (flag <= 0) return 0;
    else
        return 1;    
}

//<M>        ->     <T> - <M> | <T>
int M_parse(myScanner scanIt, Treeptr parentNode ) {
    //note that there are two M nodes in this block of codes
    Treeptr tempNode,mNode;
    int flag = 1;
    tempNode = buildTree( "<T>","", parentNode);
    if(T_parse(scanIt, tempNode)==1){
        if(launch (get_next_token, scanIt, parentNode)==0) flag--;
    }
    else flag--;
    
    if (strstr(t->tokenVal,"-")!=NULL) {
        strcpy(parentNode->value,t->tokenVal);
        tempNode = buildTree( "<M>","", parentNode);
        if(launch (get_next_token, scanIt, parentNode)==0) flag--;
        if(M_parse(scanIt, tempNode)==0) flag--;
    }
    
    if (flag <= 0) return 0;
    else
        return 1;    
}
//<T>        ->      <F> * <T> | <F> / <T> | <F>
int T_parse(myScanner scanIt, Treeptr parentNode ) {
    Treeptr tempNode,fNode;
    fNode = buildTree( "<F>","", parentNode);
    int flag = 1;

    if(F_parse(scanIt, fNode)==1){
        if(launch (get_next_token, scanIt, parentNode)==0) flag--;
    }
    else flag--;
    
    if (strstr(t->tokenVal,"*")!=NULL || strstr(t->tokenVal,"/")!=NULL) {
        strcpy(parentNode->value,t->tokenVal);
        if(launch (get_next_token, scanIt, tempNode)==0) flag--;
        tempNode = buildTree( "<T>","", parentNode);
        if(T_parse(scanIt, tempNode)==0) flag--;
    }
    
    if (flag <= 0) return 0;
    else
        return 1;    
}
//<F>        ->      - <F> | <R>
int F_parse(myScanner scanIt, Treeptr parentNode ) {
    Treeptr tempNode;
    int flag = 1;

    if (strstr(t->tokenVal,"-")!=NULL) {
        strcpy(parentNode->value,t->tokenVal);
        if(launch (get_next_token, scanIt, tempNode)==0) flag--;
        tempNode = buildTree( "<F>","", parentNode);        
        if(F_parse(scanIt, tempNode)==0) flag--;
    } else {
        tempNode = buildTree( "<R>","", parentNode);
        if(R_parse(scanIt, tempNode)==0) flag--;
    }
    
    if (flag <= 0) return 0;
    else
        return 1;    
}
//<R>        ->      [ <expr> ] | Identifier | Number   
int R_parse(myScanner scanIt, Treeptr parentNode ) {
    Treeptr tempNode;
    int flag = 1;
    if (t->tokenType==idCode) {
        tempNode = buildTree( "<ID>", t->tokenVal, parentNode);
        if(launch (get_next_token, scanIt, tempNode)==0) flag--;
    }
    if (t->tokenType==intCode) {
        tempNode = buildTree( "<INT>", t->tokenVal, parentNode);
        if(launch (get_next_token, scanIt, tempNode)==0) flag--;
    }
    else {
        if (strstr(t->tokenVal,"[")!=NULL) {
            if(launch (get_next_token, scanIt, parentNode)==0) flag--;
        }
        else
        {
            fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing '['. \n", t->tokenLine-1);
            flag --;
        }
        
        if (strstr(t->tokenVal,"-")!=NULL || strstr(t->tokenVal,"[")!=NULL || t->tokenType==idCode || t->tokenType==intCode) {
            tempNode = buildTree( "<expr>","", parentNode);
            if(expr_parse(scanIt, tempNode)==0) flag--;
        }
        else
        {
            fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing <expr>. \n", t->tokenLine-1);
            flag --;
        }
        
        if (strstr(t->tokenVal,"]")!=NULL) {
            if(launch (get_next_token, scanIt, tempNode)==0) flag--;
        }
        else
        {
            fprintf(stderr, "[ERROR : line %d] Incorrect syntax. Missing ']'. \n", t->tokenLine-1);
            flag --;
        }
    }
    if (flag <= 0) return 0;
    else
        return 1;    
}

//************************


//************************


//************************
