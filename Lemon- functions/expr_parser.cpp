/*
** 2000-05-29
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Driver template for the LEMON parser generator.
**
** The "lemon" program processes an LALR(1) input grammar file, then uses
** this template to construct a parser.  The "lemon" program inserts text
** at each "%%" line.  Also, any "P-a-r-s-e" identifer prefix (without the
** interstitial "-" characters) contained in this template is changed into
** the value of the %name directive from the grammar.  Otherwise, the content
** of this template is copied straight through into the generate parser
** source file.
**
** The following is the concatenation of all %include directives from the
** input grammar file:
*/
#include <stdio.h>
/************ Begin %include sections from the grammar ************************/
#line 2 "gram.y"

  #include <stdio.h>
  #include "utils.h"
  #include "ast.h"
  #include <assert.h>
  extern int yylineno;

#line 36 "expr_parser.cpp"
/**************** End of %include directives **********************************/
/* These constants specify the various numeric values for terminal symbols
** in a format understandable to "makeheaders".  This section is blank unless
** "lemon" is run with the "-m" command-line option.
***************** Begin makeheaders token definitions *************************/
/**************** End makeheaders token definitions ***************************/

/* The next sections is a series of control #defines.
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used to store the integer codes
**                       that represent terminal and non-terminal symbols.
**                       "unsigned char" is used if there are fewer than
**                       256 symbols.  Larger types otherwise.
**    YYNOCODE           is a number of type YYCODETYPE that is not used for
**                       any terminal or nonterminal symbol.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       (also known as: "terminal symbols") have fall-back
**                       values which should be used if the original symbol
**                       would not parse.  This permits keywords to sometimes
**                       be used as identifiers, for example.
**    YYACTIONTYPE       is the data type used for "action codes" - numbers
**                       that indicate what to do in response to the next
**                       token.
**    ParseTOKENTYPE     is the data type used for minor type for terminal
**                       symbols.  Background: A "minor type" is a semantic
**                       value associated with a terminal or non-terminal
**                       symbols.  For example, for an "ID" terminal symbol,
**                       the minor type might be the name of the identifier.
**                       Each non-terminal can have a different minor type.
**                       Terminal symbols all have the same minor type, though.
**                       This macros defines the minor type for terminal 
**                       symbols.
**    YYMINORTYPE        is the data type used for all minor types.
**                       This is typically a union of many types, one of
**                       which is ParseTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ParseARG_SDECL     A static variable declaration for the %extra_argument
**    ParseARG_PDECL     A parameter declaration for the %extra_argument
**    ParseARG_STORE     Code to store %extra_argument into yypParser
**    ParseARG_FETCH     Code to extract %extra_argument from yypParser
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_MIN_REDUCE      Maximum value for reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 53
#define YYACTIONTYPE unsigned char
#define ParseTOKENTYPE  Token* 
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
  int yy20;
  Expr* yy42;
  Statement* yy48;
  Arguments* yy65;
  Parameters* yy83;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL
#define ParseARG_PDECL
#define ParseARG_FETCH
#define ParseARG_STORE
#define YYNSTATE             64
#define YYNRULE              53
#define YY_MAX_SHIFT         63
#define YY_MIN_SHIFTREDUCE   97
#define YY_MAX_SHIFTREDUCE   149
#define YY_MIN_REDUCE        150
#define YY_MAX_REDUCE        202
#define YY_ERROR_ACTION      203
#define YY_ACCEPT_ACTION     204
#define YY_NO_ACTION         205
/************* End control #defines *******************************************/

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N <= YY_MAX_SHIFT             Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   N between YY_MIN_SHIFTREDUCE       Shift to an arbitrary state then
**     and YY_MAX_SHIFTREDUCE           reduce by rule N-YY_MIN_SHIFTREDUCE.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE

**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
**
*********** Begin parsing tables **********************************************/
#define YY_ACTTAB_COUNT (186)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */   102,  106,  105,   22,  104,  109,    9,  103,  108,  107,
 /*    10 */   111,  110,  102,  106,  105,   24,  104,  109,   17,  103,
 /*    20 */   108,  107,  111,  110,  123,  106,  105,  145,  104,  109,
 /*    30 */   100,  103,  108,  107,  111,  110,  144,  120,  106,  105,
 /*    40 */    28,  104,  109,    3,  103,  108,  107,  111,  110,  101,
 /*    50 */   106,  105,  122,  104,  109,    6,  103,  108,  107,  111,
 /*    60 */   110,    4,  106,  105,   23,   27,  109,  112,   29,  108,
 /*    70 */   107,  111,  110,   62,   53,  204,    2,   98,   59,   57,
 /*    80 */   121,   23,  126,   16,   62,   53,   39,   48,  146,   59,
 /*    90 */    57,   32,   23,   30,   16,   35,   48,  146,   56,  132,
 /*   100 */    45,   35,   48,  146,   58,  132,   45,  149,   18,   13,
 /*   110 */    12,   11,   10,   21,   20,   52,   47,   48,  146,   40,
 /*   120 */    48,  146,   41,   48,  146,  115,   42,   48,  146,   49,
 /*   130 */   146,  147,   43,   48,  146,  114,   44,   48,  146,   37,
 /*   140 */    48,  146,   46,   48,  146,   38,   48,  146,   51,   48,
 /*   150 */   146,  128,  129,  130,   36,  148,   50,  146,   60,    5,
 /*   160 */     1,   98,   54,   63,    5,   21,   20,    7,   15,   14,
 /*   170 */    31,   21,   20,   21,   20,  113,   19,   26,   25,   33,
 /*   180 */    55,    8,  125,   34,   61,  150,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    29,   30,   31,   32,   33,   34,    4,   36,   37,   38,
 /*    10 */    39,   40,   29,   30,   31,   32,   33,   34,   16,   36,
 /*    20 */    37,   38,   39,   40,   29,   30,   31,   45,   33,   34,
 /*    30 */     1,   36,   37,   38,   39,   40,   45,   29,   30,   31,
 /*    40 */     9,   33,   34,   51,   36,   37,   38,   39,   40,   29,
 /*    50 */    30,   31,   35,   33,   34,   51,   36,   37,   38,   39,
 /*    60 */    40,   51,   30,   31,   10,   33,   34,   37,   36,   37,
 /*    70 */    38,   39,   40,    2,    3,   49,   50,   51,    7,    8,
 /*    80 */    35,   10,   11,   12,    2,    3,   43,   44,   45,    7,
 /*    90 */     8,   51,   10,    5,   12,   43,   44,   45,   46,   47,
 /*   100 */    48,   43,   44,   45,   46,   47,   48,    3,    4,   19,
 /*   110 */    20,   21,   22,   23,   24,   42,   43,   44,   45,   43,
 /*   120 */    44,   45,   43,   44,   45,    3,   43,   44,   45,   44,
 /*   130 */    45,   27,   43,   44,   45,    3,   43,   44,   45,   43,
 /*   140 */    44,   45,   43,   44,   45,   43,   44,   45,   43,   44,
 /*   150 */    45,   13,   14,   15,    6,    5,   44,   45,   50,   51,
 /*   160 */    50,   51,   41,   50,   51,   23,   24,    4,   17,   18,
 /*   170 */     5,   23,   24,   23,   24,    5,    6,   25,   26,    5,
 /*   180 */     6,    4,   11,    4,    3,    0,
};
#define YY_SHIFT_USE_DFLT (-1)
#define YY_SHIFT_COUNT (63)
#define YY_SHIFT_MIN   (0)
#define YY_SHIFT_MAX   (185)
static const short yy_shift_ofst[] = {
 /*     0 */    29,   71,   82,   82,   82,   82,   82,  104,  104,  104,
 /*    10 */   104,  104,  104,  104,  104,  104,  104,  104,  104,  104,
 /*    20 */   104,  104,   29,   29,   29,  104,  104,   31,   29,   31,
 /*    30 */    29,   29,   54,   29,  122,   90,  138,  148,  150,  142,
 /*    40 */   142,  142,  142,  142,  142,  151,  142,  142,  152,  152,
 /*    50 */   152,  142,  170,    2,  174,  132,   88,  163,  165,  177,
 /*    60 */   171,  179,  181,  185,
};
#define YY_REDUCE_USE_DFLT (-30)
#define YY_REDUCE_COUNT (34)
#define YY_REDUCE_MIN   (-29)
#define YY_REDUCE_MAX   (121)
static const signed char yy_reduce_ofst[] = {
 /*     0 */    26,  -29,  -17,   -5,    8,   20,   32,   52,   58,   73,
 /*    10 */    43,   76,   79,   83,   89,   93,   96,   99,  102,  105,
 /*    20 */    85,  112,  108,  110,  113,  -18,   -9,   17,   -8,   45,
 /*    30 */     4,   10,   30,   40,  121,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   152,  203,  203,  203,  203,  151,  203,  203,  203,  172,
 /*    10 */   203,  203,  203,  203,  203,  203,  203,  203,  203,  203,
 /*    20 */   203,  203,  152,  152,  152,  203,  203,  177,  203,  177,
 /*    30 */   203,  203,  203,  203,  169,  193,  203,  180,  203,  192,
 /*    40 */   191,  190,  189,  187,  186,  188,  184,  171,  196,  195,
 /*    50 */   194,  170,  203,  203,  203,  203,  203,  203,  203,  203,
 /*    60 */   203,  203,  203,  203,
};
/********** End of lemon-generated parsing tables *****************************/

/* The next table maps tokens (terminal symbols) into fallback tokens.  
** If a construct like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
**
** This feature can be used, for example, to cause some keywords in a language
** to revert to identifiers if they keyword does not apply in the context where
** it appears.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
**
** After the "shift" half of a SHIFTREDUCE action, the stateno field
** actually contains the reduce action for the second half of the
** SHIFTREDUCE.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  int yyidx;                    /* Index of top element in stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyidxMax;                 /* Maximum value of yyidx */
#endif
#ifndef YYNOERRORRECOVERY
  int yyerrcnt;                 /* Shifts left before out of the error */
#endif
  ParseARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void ParseTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "TK_EOL",        "KW_FUNC",       "TK_ID",       
  "TK_LEFT_PAR",   "TK_RIGHT_PAR",  "OP_COMMA",      "KW_WHILE",    
  "KW_IF",         "KW_ELSE",       "TK_OPEN_BRAKET",  "TK_CLOSE_BRAKET",
  "KW_PRINT",      "KW_DEC",        "KW_HEX",        "KW_BIN",      
  "OP_ASSIGN",     "OP_EQUAL",      "OP_NOT_EQUAL",  "OP_GREATER_THAN",
  "OP_LESS_THAN",  "OP_GREATER_OR_EQUAL",  "OP_LESS_OR_EQUAL",  "OP_ADD",      
  "OP_SUB",        "OP_MUL",        "OP_DIV",        "TK_NUMBER",   
  "error",         "stmt",          "assign_st",     "print_st",    
  "stmts",         "embededd_statement",  "while_st",      "optional_else",
  "local_variable_declaration",  "braket_block_stmts",  "if_st",         "function_call",
  "function_declaration",  "fixed_parameters",  "arguments",     "expr",        
  "term",          "factor",        "conditional_expr",  "equality_expr",
  "rational_expr",  "input",         "eols_op",       "eols",        
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "input ::= eols_op stmts eols_op",
 /*   1 */ "eols_op ::= eols",
 /*   2 */ "eols_op ::=",
 /*   3 */ "eols ::= TK_EOL",
 /*   4 */ "stmts ::= stmts eols stmt",
 /*   5 */ "stmts ::= stmt",
 /*   6 */ "stmt ::= local_variable_declaration",
 /*   7 */ "stmt ::= embededd_statement",
 /*   8 */ "local_variable_declaration ::= print_st",
 /*   9 */ "local_variable_declaration ::= assign_st",
 /*  10 */ "embededd_statement ::= if_st",
 /*  11 */ "embededd_statement ::= braket_block_stmts",
 /*  12 */ "embededd_statement ::= while_st",
 /*  13 */ "embededd_statement ::= function_declaration",
 /*  14 */ "embededd_statement ::= function_call",
 /*  15 */ "function_declaration ::= KW_FUNC TK_ID TK_LEFT_PAR fixed_parameters TK_RIGHT_PAR eols braket_block_stmts",
 /*  16 */ "function_call ::= TK_ID TK_LEFT_PAR arguments TK_RIGHT_PAR",
 /*  17 */ "fixed_parameters ::= fixed_parameters OP_COMMA TK_ID",
 /*  18 */ "fixed_parameters ::= TK_ID",
 /*  19 */ "fixed_parameters ::=",
 /*  20 */ "arguments ::= arguments OP_COMMA expr",
 /*  21 */ "arguments ::= expr",
 /*  22 */ "arguments ::=",
 /*  23 */ "while_st ::= KW_WHILE TK_LEFT_PAR conditional_expr TK_RIGHT_PAR eols stmt",
 /*  24 */ "if_st ::= KW_IF TK_LEFT_PAR conditional_expr TK_RIGHT_PAR eols local_variable_declaration optional_else",
 /*  25 */ "if_st ::= KW_IF TK_LEFT_PAR conditional_expr TK_RIGHT_PAR eols embededd_statement optional_else",
 /*  26 */ "optional_else ::= KW_ELSE eols stmt",
 /*  27 */ "optional_else ::=",
 /*  28 */ "braket_block_stmts ::= TK_OPEN_BRAKET eols_op stmts eols_op TK_CLOSE_BRAKET",
 /*  29 */ "braket_block_stmts ::= TK_OPEN_BRAKET eols_op TK_CLOSE_BRAKET",
 /*  30 */ "print_st ::= KW_PRINT expr",
 /*  31 */ "print_st ::= KW_PRINT expr OP_COMMA KW_DEC",
 /*  32 */ "print_st ::= KW_PRINT expr OP_COMMA KW_HEX",
 /*  33 */ "print_st ::= KW_PRINT expr OP_COMMA KW_BIN",
 /*  34 */ "assign_st ::= TK_ID OP_ASSIGN expr",
 /*  35 */ "conditional_expr ::= equality_expr",
 /*  36 */ "equality_expr ::= rational_expr OP_EQUAL expr",
 /*  37 */ "equality_expr ::= rational_expr OP_NOT_EQUAL expr",
 /*  38 */ "equality_expr ::= rational_expr",
 /*  39 */ "rational_expr ::= expr OP_GREATER_THAN expr",
 /*  40 */ "rational_expr ::= expr OP_LESS_THAN expr",
 /*  41 */ "rational_expr ::= expr OP_GREATER_OR_EQUAL expr",
 /*  42 */ "rational_expr ::= expr OP_LESS_OR_EQUAL expr",
 /*  43 */ "rational_expr ::= expr",
 /*  44 */ "expr ::= expr OP_ADD term",
 /*  45 */ "expr ::= expr OP_SUB term",
 /*  46 */ "expr ::= term",
 /*  47 */ "term ::= term OP_MUL factor",
 /*  48 */ "term ::= term OP_DIV factor",
 /*  49 */ "term ::= factor",
 /*  50 */ "factor ::= TK_NUMBER",
 /*  51 */ "factor ::= TK_LEFT_PAR expr TK_RIGHT_PAR",
 /*  52 */ "factor ::= TK_ID",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.
*/
static void yyGrowStack(yyParser *p){
  int newSize;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  if( pNew ){
    p->yystack = pNew;
    p->yystksz = newSize;
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows to %d entries!\n",
              yyTracePrompt, p->yystksz);
    }
#endif
  }
}
#endif

/* Datatype of the argument to the memory allocated passed as the
** second argument to ParseAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to Parse and ParseFree.
*/
void *ParseAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( pParser ){
    pParser->yyidx = -1;
#ifdef YYTRACKMAXSTACKDEPTH
    pParser->yyidxMax = 0;
#endif
#if YYSTACKDEPTH<=0
    pParser->yystack = NULL;
    pParser->yystksz = 0;
    yyGrowStack(pParser);
#endif
  }
  return pParser;
}

/* The following function deletes the "minor type" or semantic value
** associated with a symbol.  The symbol can be either a terminal
** or nonterminal. "yymajor" is the symbol code, and "yypminor" is
** a pointer to the value to be deleted.  The code used to do the 
** deletions is derived from the %destructor and/or %token_destructor
** directives of the input grammar.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  ParseARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are *not* used
    ** inside the C code.
    */
/********* Begin destructor definitions ***************************************/
/********* End destructor definitions *****************************************/
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
*/
static void yy_pop_parser_stack(yyParser *pParser){
  yyStackEntry *yytos;
  assert( pParser->yyidx>=0 );
  yytos = &pParser->yystack[pParser->yyidx--];
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yy_destructor(pParser, yytos->major, &yytos->minor);
}

/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void ParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
#ifndef YYPARSEFREENEVERNULL
  if( pParser==0 ) return;
#endif
  while( pParser->yyidx>=0 ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  free(pParser->yystack);
#endif
  (*freeProc)((void*)pParser);
}

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int ParseStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyidxMax;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
*/
static int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  if( stateno>=YY_MIN_REDUCE ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
  do{
    i = yy_shift_ofst[stateno];
    if( i==YY_SHIFT_USE_DFLT ) return yy_default[stateno];
    assert( iLookAhead!=YYNOCODE );
    i += iLookAhead;
    if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
      if( iLookAhead>0 ){
#ifdef YYFALLBACK
        YYCODETYPE iFallback;            /* Fallback token */
        if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
               && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
          }
#endif
          assert( yyFallback[iFallback]==0 ); /* Fallback loop must terminate */
          iLookAhead = iFallback;
          continue;
        }
#endif
#ifdef YYWILDCARD
        {
          int j = i - iLookAhead + YYWILDCARD;
          if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
            j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
            j<YY_ACTTAB_COUNT &&
#endif
            yy_lookahead[j]==YYWILDCARD
          ){
#ifndef NDEBUG
            if( yyTraceFILE ){
              fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
                 yyTracePrompt, yyTokenName[iLookAhead],
                 yyTokenName[YYWILDCARD]);
            }
#endif /* NDEBUG */
            return yy_action[j];
          }
        }
#endif /* YYWILDCARD */
      }
      return yy_default[stateno];
    }else{
      return yy_action[i];
    }
  }while(1);
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser){
   ParseARG_FETCH;
   yypParser->yyidx--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
/******** Begin %stack_overflow code ******************************************/
/******** End %stack_overflow code ********************************************/
   ParseARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%sShift '%s', go to state %d\n",
         yyTracePrompt,yyTokenName[yypParser->yystack[yypParser->yyidx].major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%sShift '%s'\n",
         yyTracePrompt,yyTokenName[yypParser->yystack[yypParser->yyidx].major]);
    }
  }
}
#else
# define yyTraceShift(X,Y)
#endif

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  ParseTOKENTYPE yyMinor        /* The minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yyidx++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( yypParser->yyidx>yypParser->yyidxMax ){
    yypParser->yyidxMax = yypParser->yyidx;
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yyidx>=YYSTACKDEPTH ){
    yyStackOverflow(yypParser);
    return;
  }
#else
  if( yypParser->yyidx>=yypParser->yystksz ){
    yyGrowStack(yypParser);
    if( yypParser->yyidx>=yypParser->yystksz ){
      yyStackOverflow(yypParser);
      return;
    }
  }
#endif
  yytos = &yypParser->yystack[yypParser->yyidx];
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor.yy0 = yyMinor;
  yyTraceShift(yypParser, yyNewState);
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 49, 3 },
  { 50, 1 },
  { 50, 0 },
  { 51, 1 },
  { 32, 3 },
  { 32, 1 },
  { 29, 1 },
  { 29, 1 },
  { 36, 1 },
  { 36, 1 },
  { 33, 1 },
  { 33, 1 },
  { 33, 1 },
  { 33, 1 },
  { 33, 1 },
  { 40, 7 },
  { 39, 4 },
  { 41, 3 },
  { 41, 1 },
  { 41, 0 },
  { 42, 3 },
  { 42, 1 },
  { 42, 0 },
  { 34, 6 },
  { 38, 7 },
  { 38, 7 },
  { 35, 3 },
  { 35, 0 },
  { 37, 5 },
  { 37, 3 },
  { 31, 2 },
  { 31, 4 },
  { 31, 4 },
  { 31, 4 },
  { 30, 3 },
  { 46, 1 },
  { 47, 3 },
  { 47, 3 },
  { 47, 1 },
  { 48, 3 },
  { 48, 3 },
  { 48, 3 },
  { 48, 3 },
  { 48, 1 },
  { 43, 3 },
  { 43, 3 },
  { 43, 1 },
  { 44, 3 },
  { 44, 3 },
  { 44, 1 },
  { 45, 1 },
  { 45, 3 },
  { 45, 1 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  int yyruleno                 /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  ParseARG_FETCH;
  yymsp = &yypParser->yystack[yypParser->yyidx];
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno>=0 
        && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    fprintf(yyTraceFILE, "%sReduce [%s], go to state %d.\n", yyTracePrompt,
      yyRuleName[yyruleno], yymsp[-yysize].stateno);
  }
#endif /* NDEBUG */

  /* Check that the stack is large enough to grow by a single entry
  ** if the RHS of the rule is empty.  This ensures that there is room
  ** enough on the stack to push the LHS value */
  if( yyRuleInfo[yyruleno].nrhs==0 ){
#ifdef YYTRACKMAXSTACKDEPTH
    if( yypParser->yyidx>yypParser->yyidxMax ){
      yypParser->yyidxMax = yypParser->yyidx;
    }
#endif
#if YYSTACKDEPTH>0 
    if( yypParser->yyidx>=YYSTACKDEPTH-1 ){
      yyStackOverflow(yypParser);
      return;
    }
#else
    if( yypParser->yyidx>=yypParser->yystksz-1 ){
      yyGrowStack(yypParser);
      if( yypParser->yyidx>=yypParser->yystksz-1 ){
        yyStackOverflow(yypParser);
        return;
      }
    }
#endif
  }

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
/********** Begin reduce actions **********************************************/
        YYMINORTYPE yylhsminor;
      case 0: /* input ::= eols_op stmts eols_op */
#line 46 "gram.y"
{ yymsp[-1].minor.yy48->exec(); }
#line 889 "expr_parser.cpp"
        break;
      case 4: /* stmts ::= stmts eols stmt */
#line 53 "gram.y"
{ yylhsminor.yy48 = yymsp[-2].minor.yy48; ((BlockStatement *)yylhsminor.yy48)->addStatement(yymsp[0].minor.yy48); }
#line 894 "expr_parser.cpp"
  yymsp[-2].minor.yy48 = yylhsminor.yy48;
        break;
      case 5: /* stmts ::= stmt */
#line 54 "gram.y"
{ yylhsminor.yy48 = new BlockStatement; ((BlockStatement *)yylhsminor.yy48)->addStatement(yymsp[0].minor.yy48); }
#line 900 "expr_parser.cpp"
  yymsp[0].minor.yy48 = yylhsminor.yy48;
        break;
      case 6: /* stmt ::= local_variable_declaration */
      case 7: /* stmt ::= embededd_statement */ yytestcase(yyruleno==7);
      case 8: /* local_variable_declaration ::= print_st */ yytestcase(yyruleno==8);
      case 9: /* local_variable_declaration ::= assign_st */ yytestcase(yyruleno==9);
      case 10: /* embededd_statement ::= if_st */ yytestcase(yyruleno==10);
      case 11: /* embededd_statement ::= braket_block_stmts */ yytestcase(yyruleno==11);
      case 12: /* embededd_statement ::= while_st */ yytestcase(yyruleno==12);
      case 13: /* embededd_statement ::= function_declaration */ yytestcase(yyruleno==13);
      case 14: /* embededd_statement ::= function_call */ yytestcase(yyruleno==14);
#line 56 "gram.y"
{ yylhsminor.yy48 = yymsp[0].minor.yy48; }
#line 914 "expr_parser.cpp"
  yymsp[0].minor.yy48 = yylhsminor.yy48;
        break;
      case 15: /* function_declaration ::= KW_FUNC TK_ID TK_LEFT_PAR fixed_parameters TK_RIGHT_PAR eols braket_block_stmts */
#line 68 "gram.y"
{ yymsp[-6].minor.yy48 = new FunctionDeclaration(yymsp[-5].minor.yy0->char_value,yymsp[-3].minor.yy83,yymsp[0].minor.yy48); }
#line 920 "expr_parser.cpp"
        break;
      case 16: /* function_call ::= TK_ID TK_LEFT_PAR arguments TK_RIGHT_PAR */
#line 70 "gram.y"
{ yylhsminor.yy48 = new FunctionCall(yymsp[-3].minor.yy0->char_value, yymsp[-1].minor.yy65); }
#line 925 "expr_parser.cpp"
  yymsp[-3].minor.yy48 = yylhsminor.yy48;
        break;
      case 17: /* fixed_parameters ::= fixed_parameters OP_COMMA TK_ID */
#line 72 "gram.y"
{ yylhsminor.yy83 = yymsp[-2].minor.yy83; ((Parameters*)yylhsminor.yy83)->addParameter(yymsp[0].minor.yy0->char_value); }
#line 931 "expr_parser.cpp"
  yymsp[-2].minor.yy83 = yylhsminor.yy83;
        break;
      case 18: /* fixed_parameters ::= TK_ID */
#line 73 "gram.y"
{ yylhsminor.yy83 = new Parameters(); ((Parameters*)yylhsminor.yy83)->addParameter(yymsp[0].minor.yy0->char_value); }
#line 937 "expr_parser.cpp"
  yymsp[0].minor.yy83 = yylhsminor.yy83;
        break;
      case 19: /* fixed_parameters ::= */
#line 74 "gram.y"
{ yymsp[1].minor.yy83 = NULL; }
#line 943 "expr_parser.cpp"
        break;
      case 20: /* arguments ::= arguments OP_COMMA expr */
#line 76 "gram.y"
{ yylhsminor.yy65=yymsp[-2].minor.yy65; ((Arguments*)yylhsminor.yy65)->addArgument(yymsp[0].minor.yy42); }
#line 948 "expr_parser.cpp"
  yymsp[-2].minor.yy65 = yylhsminor.yy65;
        break;
      case 21: /* arguments ::= expr */
#line 77 "gram.y"
{ yylhsminor.yy65 = new Arguments(); ((Arguments*)yylhsminor.yy65)->addArgument(yymsp[0].minor.yy42); }
#line 954 "expr_parser.cpp"
  yymsp[0].minor.yy65 = yylhsminor.yy65;
        break;
      case 22: /* arguments ::= */
#line 78 "gram.y"
{ yymsp[1].minor.yy65 = NULL; }
#line 960 "expr_parser.cpp"
        break;
      case 23: /* while_st ::= KW_WHILE TK_LEFT_PAR conditional_expr TK_RIGHT_PAR eols stmt */
#line 80 "gram.y"
{ yymsp[-5].minor.yy48 = new WhileStatement(yymsp[-3].minor.yy42,yymsp[0].minor.yy48); }
#line 965 "expr_parser.cpp"
        break;
      case 24: /* if_st ::= KW_IF TK_LEFT_PAR conditional_expr TK_RIGHT_PAR eols local_variable_declaration optional_else */
      case 25: /* if_st ::= KW_IF TK_LEFT_PAR conditional_expr TK_RIGHT_PAR eols embededd_statement optional_else */ yytestcase(yyruleno==25);
#line 82 "gram.y"
{ yymsp[-6].minor.yy48 = new IfStatement(yymsp[-4].minor.yy42,yymsp[-1].minor.yy48,yymsp[0].minor.yy48); }
#line 971 "expr_parser.cpp"
        break;
      case 26: /* optional_else ::= KW_ELSE eols stmt */
#line 85 "gram.y"
{ yymsp[-2].minor.yy48 = yymsp[0].minor.yy48; }
#line 976 "expr_parser.cpp"
        break;
      case 27: /* optional_else ::= */
#line 86 "gram.y"
{ yymsp[1].minor.yy48 = NULL; }
#line 981 "expr_parser.cpp"
        break;
      case 28: /* braket_block_stmts ::= TK_OPEN_BRAKET eols_op stmts eols_op TK_CLOSE_BRAKET */
#line 88 "gram.y"
{ yymsp[-4].minor.yy48 = yymsp[-2].minor.yy48; }
#line 986 "expr_parser.cpp"
        break;
      case 29: /* braket_block_stmts ::= TK_OPEN_BRAKET eols_op TK_CLOSE_BRAKET */
#line 89 "gram.y"
{ yymsp[-2].minor.yy48 = NULL; }
#line 991 "expr_parser.cpp"
        break;
      case 30: /* print_st ::= KW_PRINT expr */
#line 91 "gram.y"
{ yymsp[-1].minor.yy48 = new PrintStatement(yymsp[0].minor.yy42); }
#line 996 "expr_parser.cpp"
        break;
      case 31: /* print_st ::= KW_PRINT expr OP_COMMA KW_DEC */
#line 92 "gram.y"
{ yymsp[-3].minor.yy48 = new PrintStatement(yymsp[-2].minor.yy42, DEC); }
#line 1001 "expr_parser.cpp"
        break;
      case 32: /* print_st ::= KW_PRINT expr OP_COMMA KW_HEX */
#line 93 "gram.y"
{ yymsp[-3].minor.yy48 = new PrintStatement(yymsp[-2].minor.yy42, HEX); }
#line 1006 "expr_parser.cpp"
        break;
      case 33: /* print_st ::= KW_PRINT expr OP_COMMA KW_BIN */
#line 94 "gram.y"
{ yymsp[-3].minor.yy48 = new PrintStatement(yymsp[-2].minor.yy42, BIN); }
#line 1011 "expr_parser.cpp"
        break;
      case 34: /* assign_st ::= TK_ID OP_ASSIGN expr */
#line 96 "gram.y"
{ yylhsminor.yy48= new AssignStatement(yymsp[-2].minor.yy0->char_value, yymsp[0].minor.yy42); free(yymsp[-2].minor.yy0); }
#line 1016 "expr_parser.cpp"
  yymsp[-2].minor.yy48 = yylhsminor.yy48;
        break;
      case 35: /* conditional_expr ::= equality_expr */
      case 46: /* expr ::= term */ yytestcase(yyruleno==46);
#line 98 "gram.y"
{ yylhsminor.yy42 = yymsp[0].minor.yy42; }
#line 1023 "expr_parser.cpp"
  yymsp[0].minor.yy42 = yylhsminor.yy42;
        break;
      case 36: /* equality_expr ::= rational_expr OP_EQUAL expr */
#line 100 "gram.y"
{ yylhsminor.yy42 = new EqualExpr(yymsp[-2].minor.yy42,yymsp[0].minor.yy42); }
#line 1029 "expr_parser.cpp"
  yymsp[-2].minor.yy42 = yylhsminor.yy42;
        break;
      case 37: /* equality_expr ::= rational_expr OP_NOT_EQUAL expr */
#line 101 "gram.y"
{ yylhsminor.yy42 = new NotEqualExpr(yymsp[-2].minor.yy42,yymsp[0].minor.yy42); }
#line 1035 "expr_parser.cpp"
  yymsp[-2].minor.yy42 = yylhsminor.yy42;
        break;
      case 38: /* equality_expr ::= rational_expr */
      case 43: /* rational_expr ::= expr */ yytestcase(yyruleno==43);
      case 49: /* term ::= factor */ yytestcase(yyruleno==49);
#line 102 "gram.y"
{ yylhsminor.yy42=yymsp[0].minor.yy42; }
#line 1043 "expr_parser.cpp"
  yymsp[0].minor.yy42 = yylhsminor.yy42;
        break;
      case 39: /* rational_expr ::= expr OP_GREATER_THAN expr */
#line 104 "gram.y"
{ yylhsminor.yy42 = new GreaExpr(yymsp[-2].minor.yy42, yymsp[0].minor.yy42); }
#line 1049 "expr_parser.cpp"
  yymsp[-2].minor.yy42 = yylhsminor.yy42;
        break;
      case 40: /* rational_expr ::= expr OP_LESS_THAN expr */
#line 105 "gram.y"
{ yylhsminor.yy42 = new LessExpr(yymsp[-2].minor.yy42, yymsp[0].minor.yy42); }
#line 1055 "expr_parser.cpp"
  yymsp[-2].minor.yy42 = yylhsminor.yy42;
        break;
      case 41: /* rational_expr ::= expr OP_GREATER_OR_EQUAL expr */
#line 106 "gram.y"
{ yylhsminor.yy42 = new GreaEqExpr(yymsp[-2].minor.yy42, yymsp[0].minor.yy42); }
#line 1061 "expr_parser.cpp"
  yymsp[-2].minor.yy42 = yylhsminor.yy42;
        break;
      case 42: /* rational_expr ::= expr OP_LESS_OR_EQUAL expr */
#line 107 "gram.y"
{ yylhsminor.yy42 = new LessEqExpr(yymsp[-2].minor.yy42, yymsp[0].minor.yy42); }
#line 1067 "expr_parser.cpp"
  yymsp[-2].minor.yy42 = yylhsminor.yy42;
        break;
      case 44: /* expr ::= expr OP_ADD term */
#line 110 "gram.y"
{ yylhsminor.yy42 = new AddExpr(yymsp[-2].minor.yy42, yymsp[0].minor.yy42); }
#line 1073 "expr_parser.cpp"
  yymsp[-2].minor.yy42 = yylhsminor.yy42;
        break;
      case 45: /* expr ::= expr OP_SUB term */
#line 111 "gram.y"
{ yylhsminor.yy42 = new SubExpr(yymsp[-2].minor.yy42, yymsp[0].minor.yy42); }
#line 1079 "expr_parser.cpp"
  yymsp[-2].minor.yy42 = yylhsminor.yy42;
        break;
      case 47: /* term ::= term OP_MUL factor */
#line 114 "gram.y"
{ yylhsminor.yy42 = new MulExpr(yymsp[-2].minor.yy42,yymsp[0].minor.yy42); }
#line 1085 "expr_parser.cpp"
  yymsp[-2].minor.yy42 = yylhsminor.yy42;
        break;
      case 48: /* term ::= term OP_DIV factor */
#line 115 "gram.y"
{ yylhsminor.yy42 = new DivExpr(yymsp[-2].minor.yy42,yymsp[0].minor.yy42); }
#line 1091 "expr_parser.cpp"
  yymsp[-2].minor.yy42 = yylhsminor.yy42;
        break;
      case 50: /* factor ::= TK_NUMBER */
#line 118 "gram.y"
{yylhsminor.yy42 = new NumberExpr(yymsp[0].minor.yy0->int_value); free(yymsp[0].minor.yy0); }
#line 1097 "expr_parser.cpp"
  yymsp[0].minor.yy42 = yylhsminor.yy42;
        break;
      case 51: /* factor ::= TK_LEFT_PAR expr TK_RIGHT_PAR */
#line 119 "gram.y"
{ yymsp[-2].minor.yy42=yymsp[-1].minor.yy42; }
#line 1103 "expr_parser.cpp"
        break;
      case 52: /* factor ::= TK_ID */
#line 120 "gram.y"
{ yylhsminor.yy42= new VarExpr(yymsp[0].minor.yy0->char_value); free(yymsp[0].minor.yy0); }
#line 1108 "expr_parser.cpp"
  yymsp[0].minor.yy42 = yylhsminor.yy42;
        break;
      default:
      /* (1) eols_op ::= eols */ yytestcase(yyruleno==1);
      /* (2) eols_op ::= */ yytestcase(yyruleno==2);
      /* (3) eols ::= TK_EOL */ yytestcase(yyruleno==3);
        break;
/********** End reduce actions ************************************************/
  };
  assert( yyruleno>=0 && yyruleno<sizeof(yyRuleInfo)/sizeof(yyRuleInfo[0]) );
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact <= YY_MAX_SHIFTREDUCE ){
    if( yyact>YY_MAX_SHIFT ) yyact += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
    yypParser->yyidx -= yysize - 1;
    yymsp -= yysize-1;
    yymsp->stateno = (YYACTIONTYPE)yyact;
    yymsp->major = (YYCODETYPE)yygoto;
    yyTraceShift(yypParser, yyact);
  }else{
    assert( yyact == YY_ACCEPT_ACTION );
    yypParser->yyidx -= yysize;
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
/************ End %parse_failure code *****************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  ParseTOKENTYPE yyminor         /* The minor type of the error token */
){
  ParseARG_FETCH;
#define TOKEN yyminor
/************ Begin %syntax_error code ****************************************/
#line 14 "gram.y"

  printf("Syntax error at line %d \n", yylineno);
#line 1172 "expr_parser.cpp"
/************ End %syntax_error code ******************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
/*********** Begin %parse_accept code *****************************************/
#line 11 "gram.y"

  printf("parsing complete!\n");
#line 1196 "expr_parser.cpp"
/*********** End %parse_accept code *******************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ParseAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void Parse(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ParseTOKENTYPE yyminor       /* The value for the token */
  ParseARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  int yyact;            /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  /* (re)initialize the parser, if necessary */
  yypParser = (yyParser*)yyp;
  if( yypParser->yyidx<0 ){
#if YYSTACKDEPTH<=0
    if( yypParser->yystksz <=0 ){
      yyStackOverflow(yypParser);
      return;
    }
#endif
    yypParser->yyidx = 0;
#ifndef YYNOERRORRECOVERY
    yypParser->yyerrcnt = -1;
#endif
    yypParser->yystack[0].stateno = 0;
    yypParser->yystack[0].major = 0;
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sInitialize. Empty stack. State 0\n",
              yyTracePrompt);
    }
#endif
  }
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif
  ParseARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput '%s'\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact <= YY_MAX_SHIFTREDUCE ){
      if( yyact > YY_MAX_SHIFT ) yyact += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
      yy_shift(yypParser,yyact,yymajor,yyminor);
#ifndef YYNOERRORRECOVERY
      yypParser->yyerrcnt--;
#endif
      yymajor = YYNOCODE;
    }else if( yyact <= YY_MAX_REDUCE ){
      yy_reduce(yypParser,yyact-YY_MIN_REDUCE);
    }else{
      assert( yyact == YY_ERROR_ACTION );
      yyminorunion.yy0 = yyminor;
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminor);
      }
      yymx = yypParser->yystack[yypParser->yyidx].major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor, &yyminorunion);
        yymajor = YYNOCODE;
      }else{
        while(
          yypParser->yyidx >= 0 &&
          yymx != YYERRORSYMBOL &&
          (yyact = yy_find_reduce_action(
                        yypParser->yystack[yypParser->yyidx].stateno,
                        YYERRORSYMBOL)) >= YY_MIN_REDUCE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yyidx < 0 || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          yy_shift(yypParser,yyact,YYERRORSYMBOL,yyminor);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor, yyminor);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor, yyminor);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yyidx>=0 );
#ifndef NDEBUG
  if( yyTraceFILE ){
    int i;
    fprintf(yyTraceFILE,"%sReturn. Stack=",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE,"%c%s", i==1 ? '[' : ' ', 
              yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"]\n");
  }
#endif
  return;
}
