# FULL SCANNER IMPLEMENTATION
Author: Tam N Nguyen


## Execution
make : compile main program
testFronEnd [file name] : scan a file
testFrontEnd : input from keyboard or file redirection stream

Results will be saved to "parseResult"

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

## Recursive descent parsing
- This is a top-down parser
- Every non-terminal has a function
- Parsed results will be added into a binary tree
- Parsing is done left to right

## The parsed tree of test case (test2 file)
![parsed bnTree]
(https://github.com/genterist/simpleParser/blob/master/ParserBNTree-2.png)

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

## Automaton
![parser automaton]
(https://github.com/genterist/simpleParser/blob/master/Parser-2.png)

![driver table]
(https://github.com/genterist/simpleParser/blob/master/Screen%20Shot%202016-10-16%20at%2011.24.06%20AM.png)

![mapped driver table]
(https://github.com/genterist/simpleParser/blob/master/Screen%20Shot%202016-10-16%20at%2011.24.31%20AM.png)


.
