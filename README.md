# A SIMPLE COMPILER
Author: Tam N Nguyen
Github: https://github.com/genterist/simpleParser

## Forewords
This is a simple compiler demonstrating the basic concepts commonly found in most compilers. Usually, a compiler consists of a scanner, a parser and a generator each of which will be described in further details below.
There are some bugs in the codes and the final generated assembly codes are just for illustration purposes. If you are my potential employer and want to check out the full version of this program, please email your name, company name, position, contact info to genterist [at] live.com
Students! You are free to check the codes so you can understand the concepts but please do not copy the codes for your assignments.

## Execution
make : compile main program
make clean : clean up all compiled files
comp [file name] : compile a file (not that file ext of .fs16 is implicit)
comp : input from keyboard or file redirection stream

Compiler will generate:
1- Onscreen feedbacks
2- *.tree files for parsed tree
3- *.asm files for generated asm codes


Program will quit when EOF reached or there is an error
When in manual keyboard input mode, CTRL-Z (EOF signal) will terminate the program
EOF is recommended to be on a new line in programming text (a blank line at the end of each programming text, after "End" keyword)

## The Context-Free Grammar to be used
~~~~
<program>  ->     <vars> <block>
<block>    ->      Begin <vars> <stats> End
<vars>     ->      empty | Var Identifier <mvars> 
<mvars>    ->      empty | : : Identifier <mvars>
<expr>     ->      <M> + <expr> | <M>
<M>        ->      <T> - <M> | <T>
<T>        ->      <F> * <T> | <F> / <T> | <F>
<F>        ->      - <F> | <R>
<R>        ->      [ <expr> ] | Identifier | Number   
<stats>    ->      <stat>  <mStat>
<mStat>    ->      empty | <stat>  <mStat>
<stat>     ->      <in> | <out> | <block> | <if> | <loop> | <assign>
<in>       ->      Scan : Identifier .
<out>      ->      Print [ <expr>  ] .
<if>       ->      [ <expr> <RO> <expr> ]  Iff <block>             
<loop>     ->      Loop [ <expr> <RO> <expr> ] <block>
<assign>   ->      Identifier == <expr> .                    // == is one token here
<RO>       ->      >=> | <=< | = |  > | <  |  =!=           // each is one token here
~~~~

## Lexical Definitions
- All case sensitive
- Each scanner error should display the error message after "Scanner Error:"
1. Alphabet
  * All English letters (upper and lower), digits, plus the extra characters as seen below, plus WS
  * No other characters allowed and they should generate lexical errors
2. Identifiers
  * Begin with a letter and continue with any number of letters and digits 
  * no ID is longer than 8
3. Keywords (reserved, suggested individual tokens)
  * Begin End Start Stop Iff Loop Void Var Int Call Return Scan Print Program 
4. Relational operators
  * =  <  >  =!=    >=>  <=<
5. Other operators
  * == :  +  -  *  / & %
6. Delimiters
  . (  ) , { } ; [ ]
7. Integers
  * Any sequence of decimal digits, no sign
  * No number longer than 8 digits
8. Comments
  * Start with @ and end with WS

## Scanner :: Automaton
![parser automaton]
(https://github.com/genterist/simpleParser/blob/master/Parser-2.png)

![driver table]
(https://github.com/genterist/simpleParser/blob/master/Screen%20Shot%202016-10-16%20at%2011.24.06%20AM.png)

![mapped driver table]
(https://github.com/genterist/simpleParser/blob/master/Screen%20Shot%202016-10-16%20at%2011.24.31%20AM.png)

## Parser :: Recursive descent parsing
- This is a top-down parser
- Every non-terminal has a function
- Parsed results will be added into a binary tree
- Parsing is done left to right
- Parsed tree will be a Binary Tree

## Parser :: The parsed tree of test case (test2 file)
![parsed bnTree]
(https://github.com/genterist/simpleParser/blob/master/ParserBNTree-2.png)

## Compiller
- Compiler will traverse the parsed tree top down
- Compiler will generate ASM codes and save to .asm file
- Reserved variables name are T[scopenumber] and Ttemp
- Compiler uses global storage with local scope separation (no stack was used to maintain local scope variables - full implementation of this program will use stacks to manage local variables)
- Even when there are errors in programming syntax, an ASM file will still be generated
.
