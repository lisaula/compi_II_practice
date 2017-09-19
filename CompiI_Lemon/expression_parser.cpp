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
#line 1 "grammar.y"

    // #include "ast.h"
    #include<stdio.h>
    #include <iostream>
    #include <string>
    #include<assert.h>

    using namespace std;

    extern int yylineno;
#line 39 "expression_parser.cpp"
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
#define YYNOCODE 68
#define YYACTIONTYPE unsigned char
#define ParseTOKENTYPE  string * 
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
  int yy62;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL
#define ParseARG_PDECL
#define ParseARG_FETCH
#define ParseARG_STORE
#define YYNSTATE             65
#define YYNRULE              63
#define YY_MAX_SHIFT         64
#define YY_MIN_SHIFTREDUCE   111
#define YY_MAX_SHIFTREDUCE   173
#define YY_MIN_REDUCE        174
#define YY_MAX_REDUCE        236
#define YY_ERROR_ACTION      237
#define YY_ACCEPT_ACTION     238
#define YY_NO_ACTION         239
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
#define YY_ACTTAB_COUNT (195)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */   238,    2,  113,   21,   64,  117,  118,  119,  120,  121,
 /*    10 */   143,  144,  145,  146,  147,  148,  137,  161,  165,   13,
 /*    20 */    12,  149,  150,    3,  113,   39,   64,  117,  118,  119,
 /*    30 */   120,  121,  125,  126,  127,  128,  129,   53,  137,    8,
 /*    40 */   130,   61,   31,  149,  150,  112,  134,   64,  117,  118,
 /*    50 */   119,  120,  121,   10,   50,   32,   54,  162,  165,  137,
 /*    60 */    33,   54,  162,  165,  149,  150,  174,   22,   63,  135,
 /*    70 */    18,  153,   58,  169,  170,  171,  172,  173,  131,   48,
 /*    80 */     1,   52,   13,   12,  125,  126,  127,  128,  129,   46,
 /*    90 */    42,   40,   63,  152,   18,   20,   35,   54,  162,  165,
 /*   100 */   151,   21,   41,   48,  124,   16,   15,   14,   32,   54,
 /*   110 */   162,  165,   63,   46,   42,   40,   43,   20,   55,  162,
 /*   120 */   165,   20,   18,  139,   44,   36,   13,   12,   47,   37,
 /*   130 */    32,   54,  162,  165,   32,   54,  162,  165,   32,   54,
 /*   140 */   162,  165,   32,   54,  162,  165,   34,   45,   27,   59,
 /*   150 */   162,  165,  166,   38,  138,   62,  162,  165,    9,   32,
 /*   160 */    54,  162,  165,   13,   12,   28,   32,   54,  162,  165,
 /*   170 */   141,  123,    7,   25,   23,    4,    5,   24,   26,  136,
 /*   180 */    29,    6,   49,   19,   51,   30,  168,   17,   57,   56,
 /*   190 */   164,  176,   60,   11,  116,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    40,   41,   42,    4,   44,   45,   46,   47,   48,   49,
 /*    10 */    17,   18,   19,   20,   21,   22,   56,   65,   66,   26,
 /*    20 */    27,   61,   62,   41,   42,   43,   44,   45,   46,   47,
 /*    30 */    48,   49,    8,    9,   10,   11,   12,   51,   56,   60,
 /*    40 */    54,    2,   50,   61,   62,   42,   55,   44,   45,   46,
 /*    50 */    47,   48,   49,   14,   54,   63,   64,   65,   66,   56,
 /*    60 */    63,   64,   65,   66,   61,   62,    0,   53,    2,   55,
 /*    70 */     4,   15,   33,   34,   35,   36,   37,   38,    2,   13,
 /*    80 */     6,    5,   26,   27,    8,    9,   10,   11,   12,   23,
 /*    90 */    24,   25,    2,   58,    4,   50,   63,   64,   65,   66,
 /*   100 */    58,    4,   57,   13,    7,   28,   29,   30,   63,   64,
 /*   110 */    65,   66,    2,   23,   24,   25,   47,   50,   64,   65,
 /*   120 */    66,   50,    4,   58,   57,   50,   26,   27,   57,   50,
 /*   130 */    63,   64,   65,   66,   63,   64,   65,   66,   63,   64,
 /*   140 */    65,   66,   63,   64,   65,   66,   50,   48,   16,   64,
 /*   150 */    65,   66,   15,   50,   59,   64,   65,   66,    3,   63,
 /*   160 */    64,   65,   66,   26,   27,   58,   63,   64,   65,   66,
 /*   170 */     7,   52,   14,    1,   15,   14,    1,   15,   14,    1,
 /*   180 */    15,   14,    2,    6,    2,    2,    2,   31,    2,   32,
 /*   190 */     2,   67,   32,    3,    1,
};
#define YY_SHIFT_USE_DFLT (-8)
#define YY_SHIFT_COUNT (64)
#define YY_SHIFT_MIN   (-7)
#define YY_SHIFT_MAX   (193)
static const short yy_shift_ofst[] = {
 /*     0 */    90,   90,   66,   90,   39,   39,   39,   39,   39,   39,
 /*    10 */    39,   39,   39,   39,   39,   39,   39,   39,   76,   -1,
 /*    20 */    -7,   24,   97,   74,   74,  110,  118,   74,  132,   74,
 /*    30 */   155,   56,   77,   77,  137,   77,  100,  100,  100,  163,
 /*    40 */   158,  159,  161,  162,  172,  175,  164,  165,  167,  178,
 /*    50 */   180,  177,  182,  183,  156,  156,  184,  157,  186,  156,
 /*    60 */   188,  160,  156,  190,  193,
};
#define YY_REDUCE_USE_DFLT (-49)
#define YY_REDUCE_COUNT (30)
#define YY_REDUCE_MIN   (-48)
#define YY_REDUCE_MAX   (119)
static const signed char yy_reduce_ofst[] = {
 /*     0 */   -40,  -18,    3,    3,   45,   67,   71,   -8,   75,   79,
 /*    10 */    96,  103,   -3,   33,   54,   85,   91,  -48,  -14,   14,
 /*    20 */   -21,    0,   -9,   35,   42,   69,   99,   65,   95,  107,
 /*    30 */   119,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   237,  178,  237,  177,  237,  237,  237,  237,  237,  237,
 /*    10 */   237,  237,  237,  237,  237,  237,  237,  237,  237,  237,
 /*    20 */   237,  237,  237,  237,  237,  237,  237,  237,  203,  237,
 /*    30 */   196,  237,  219,  218,  237,  217,  205,  195,  185,  237,
 /*    40 */   237,  237,  237,  237,  237,  237,  237,  237,  237,  237,
 /*    50 */   237,  237,  237,  237,  223,  222,  237,  230,  237,  221,
 /*    60 */   237,  226,  220,  237,  237,
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
  "$",             "TK_EOS",        "TK_ID",         "OP_ASSIGN",   
  "KW_DECL",       "KW_STRUCT",     "TK_L_BRACE",    "TK_R_BRACE",  
  "KW_INT",        "KW_BOOL",       "KW_CHAR",       "KW_DOUBLE",   
  "KW_STRING",     "KW_IF",         "TK_L_PAR",      "TK_R_PAR",    
  "KW_ELSE",       "OP_GT",         "OP_GET",        "OP_LT",       
  "OP_LET",        "OP_EQ",         "OP_NEQ",        "KW_FOR",      
  "KW_WHILE",      "KW_PRINT",      "OP_PLUS",       "OP_MINUS",    
  "OP_MUL",        "OP_DIV",        "OP_MOD",        "OP_POW",      
  "TK_DOT",        "KW_TYPEOF",     "LIT_INT",       "LIT_BOOL",    
  "LIT_CHAR",      "LIT_DOUBLE",    "LIT_STRING",    "error",       
  "code",          "stmts",         "stmt",          "opt_stmts",   
  "st_options",    "conditional_st",  "loop_st",       "assign_st",   
  "decl_st",       "print_st",      "expr",          "type_w_id",   
  "opt_assign_st",  "attr_list",     "type",          "attr_decl",   
  "if_expr",       "bool_expr",     "block_st",      "opt_else",    
  "compare_option",  "for_st",        "while_st",      "term",        
  "ssj_term",      "factor",        "literal",     
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "code ::= stmts",
 /*   1 */ "stmts ::= stmts stmt",
 /*   2 */ "stmts ::= stmt",
 /*   3 */ "opt_stmts ::= stmts",
 /*   4 */ "opt_stmts ::=",
 /*   5 */ "stmt ::= st_options TK_EOS",
 /*   6 */ "stmt ::= conditional_st",
 /*   7 */ "stmt ::= loop_st",
 /*   8 */ "st_options ::= assign_st",
 /*   9 */ "st_options ::= decl_st",
 /*  10 */ "st_options ::= print_st",
 /*  11 */ "assign_st ::= TK_ID OP_ASSIGN expr",
 /*  12 */ "decl_st ::= KW_DECL type_w_id TK_ID opt_assign_st",
 /*  13 */ "decl_st ::= KW_DECL KW_STRUCT TK_ID TK_L_BRACE attr_list TK_R_BRACE",
 /*  14 */ "type ::= KW_INT",
 /*  15 */ "type ::= KW_BOOL",
 /*  16 */ "type ::= KW_CHAR",
 /*  17 */ "type ::= KW_DOUBLE",
 /*  18 */ "type ::= KW_STRING",
 /*  19 */ "type_w_id ::= type",
 /*  20 */ "type_w_id ::= TK_ID",
 /*  21 */ "opt_assign_st ::= OP_ASSIGN expr",
 /*  22 */ "opt_assign_st ::=",
 /*  23 */ "attr_list ::= attr_list attr_decl",
 /*  24 */ "attr_list ::= attr_decl",
 /*  25 */ "attr_decl ::= KW_DECL type TK_ID TK_EOS",
 /*  26 */ "conditional_st ::= if_expr",
 /*  27 */ "if_expr ::= KW_IF TK_L_PAR bool_expr TK_R_PAR block_st opt_else",
 /*  28 */ "opt_else ::= KW_ELSE block_st",
 /*  29 */ "opt_else ::=",
 /*  30 */ "block_st ::= TK_L_BRACE opt_stmts TK_R_BRACE",
 /*  31 */ "bool_expr ::= expr compare_option expr",
 /*  32 */ "compare_option ::= OP_GT",
 /*  33 */ "compare_option ::= OP_GET",
 /*  34 */ "compare_option ::= OP_LT",
 /*  35 */ "compare_option ::= OP_LET",
 /*  36 */ "compare_option ::= OP_EQ",
 /*  37 */ "compare_option ::= OP_NEQ",
 /*  38 */ "loop_st ::= for_st",
 /*  39 */ "loop_st ::= while_st",
 /*  40 */ "for_st ::= KW_FOR TK_L_PAR decl_st TK_EOS bool_expr TK_EOS assign_st TK_R_PAR block_st",
 /*  41 */ "while_st ::= KW_WHILE TK_L_PAR bool_expr TK_R_PAR block_st",
 /*  42 */ "print_st ::= KW_PRINT TK_L_PAR expr TK_R_PAR",
 /*  43 */ "expr ::= expr OP_PLUS term",
 /*  44 */ "expr ::= expr OP_MINUS term",
 /*  45 */ "expr ::= term",
 /*  46 */ "term ::= term OP_MUL ssj_term",
 /*  47 */ "term ::= term OP_DIV ssj_term",
 /*  48 */ "term ::= term OP_MOD ssj_term",
 /*  49 */ "term ::= ssj_term",
 /*  50 */ "ssj_term ::= ssj_term OP_POW factor",
 /*  51 */ "ssj_term ::= factor",
 /*  52 */ "factor ::= TK_ID",
 /*  53 */ "factor ::= TK_ID TK_DOT TK_ID",
 /*  54 */ "factor ::= literal",
 /*  55 */ "factor ::= TK_L_PAR expr TK_R_PAR",
 /*  56 */ "factor ::= KW_TYPEOF TK_ID",
 /*  57 */ "factor ::= KW_TYPEOF TK_ID TK_DOT TK_ID",
 /*  58 */ "literal ::= LIT_INT",
 /*  59 */ "literal ::= LIT_BOOL",
 /*  60 */ "literal ::= LIT_CHAR",
 /*  61 */ "literal ::= LIT_DOUBLE",
 /*  62 */ "literal ::= LIT_STRING",
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
  { 40, 1 },
  { 41, 2 },
  { 41, 1 },
  { 43, 1 },
  { 43, 0 },
  { 42, 2 },
  { 42, 1 },
  { 42, 1 },
  { 44, 1 },
  { 44, 1 },
  { 44, 1 },
  { 47, 3 },
  { 48, 4 },
  { 48, 6 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 51, 1 },
  { 51, 1 },
  { 52, 2 },
  { 52, 0 },
  { 53, 2 },
  { 53, 1 },
  { 55, 4 },
  { 45, 1 },
  { 56, 6 },
  { 59, 2 },
  { 59, 0 },
  { 58, 3 },
  { 57, 3 },
  { 60, 1 },
  { 60, 1 },
  { 60, 1 },
  { 60, 1 },
  { 60, 1 },
  { 60, 1 },
  { 46, 1 },
  { 46, 1 },
  { 61, 9 },
  { 62, 5 },
  { 49, 4 },
  { 50, 3 },
  { 50, 3 },
  { 50, 1 },
  { 63, 3 },
  { 63, 3 },
  { 63, 3 },
  { 63, 1 },
  { 64, 3 },
  { 64, 1 },
  { 65, 1 },
  { 65, 3 },
  { 65, 1 },
  { 65, 3 },
  { 65, 2 },
  { 65, 4 },
  { 66, 1 },
  { 66, 1 },
  { 66, 1 },
  { 66, 1 },
  { 66, 1 },
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
      default:
      /* (0) code ::= stmts */ yytestcase(yyruleno==0);
      /* (1) stmts ::= stmts stmt */ yytestcase(yyruleno==1);
      /* (2) stmts ::= stmt */ yytestcase(yyruleno==2);
      /* (3) opt_stmts ::= stmts */ yytestcase(yyruleno==3);
      /* (4) opt_stmts ::= */ yytestcase(yyruleno==4);
      /* (5) stmt ::= st_options TK_EOS */ yytestcase(yyruleno==5);
      /* (6) stmt ::= conditional_st */ yytestcase(yyruleno==6);
      /* (7) stmt ::= loop_st */ yytestcase(yyruleno==7);
      /* (8) st_options ::= assign_st */ yytestcase(yyruleno==8);
      /* (9) st_options ::= decl_st */ yytestcase(yyruleno==9);
      /* (10) st_options ::= print_st */ yytestcase(yyruleno==10);
      /* (11) assign_st ::= TK_ID OP_ASSIGN expr */ yytestcase(yyruleno==11);
      /* (12) decl_st ::= KW_DECL type_w_id TK_ID opt_assign_st */ yytestcase(yyruleno==12);
      /* (13) decl_st ::= KW_DECL KW_STRUCT TK_ID TK_L_BRACE attr_list TK_R_BRACE */ yytestcase(yyruleno==13);
      /* (14) type ::= KW_INT */ yytestcase(yyruleno==14);
      /* (15) type ::= KW_BOOL */ yytestcase(yyruleno==15);
      /* (16) type ::= KW_CHAR */ yytestcase(yyruleno==16);
      /* (17) type ::= KW_DOUBLE */ yytestcase(yyruleno==17);
      /* (18) type ::= KW_STRING */ yytestcase(yyruleno==18);
      /* (19) type_w_id ::= type */ yytestcase(yyruleno==19);
      /* (20) type_w_id ::= TK_ID */ yytestcase(yyruleno==20);
      /* (21) opt_assign_st ::= OP_ASSIGN expr */ yytestcase(yyruleno==21);
      /* (22) opt_assign_st ::= */ yytestcase(yyruleno==22);
      /* (23) attr_list ::= attr_list attr_decl */ yytestcase(yyruleno==23);
      /* (24) attr_list ::= attr_decl */ yytestcase(yyruleno==24);
      /* (25) attr_decl ::= KW_DECL type TK_ID TK_EOS */ yytestcase(yyruleno==25);
      /* (26) conditional_st ::= if_expr */ yytestcase(yyruleno==26);
      /* (27) if_expr ::= KW_IF TK_L_PAR bool_expr TK_R_PAR block_st opt_else */ yytestcase(yyruleno==27);
      /* (28) opt_else ::= KW_ELSE block_st */ yytestcase(yyruleno==28);
      /* (29) opt_else ::= */ yytestcase(yyruleno==29);
      /* (30) block_st ::= TK_L_BRACE opt_stmts TK_R_BRACE */ yytestcase(yyruleno==30);
      /* (31) bool_expr ::= expr compare_option expr */ yytestcase(yyruleno==31);
      /* (32) compare_option ::= OP_GT */ yytestcase(yyruleno==32);
      /* (33) compare_option ::= OP_GET */ yytestcase(yyruleno==33);
      /* (34) compare_option ::= OP_LT */ yytestcase(yyruleno==34);
      /* (35) compare_option ::= OP_LET */ yytestcase(yyruleno==35);
      /* (36) compare_option ::= OP_EQ */ yytestcase(yyruleno==36);
      /* (37) compare_option ::= OP_NEQ */ yytestcase(yyruleno==37);
      /* (38) loop_st ::= for_st */ yytestcase(yyruleno==38);
      /* (39) loop_st ::= while_st */ yytestcase(yyruleno==39);
      /* (40) for_st ::= KW_FOR TK_L_PAR decl_st TK_EOS bool_expr TK_EOS assign_st TK_R_PAR block_st */ yytestcase(yyruleno==40);
      /* (41) while_st ::= KW_WHILE TK_L_PAR bool_expr TK_R_PAR block_st */ yytestcase(yyruleno==41);
      /* (42) print_st ::= KW_PRINT TK_L_PAR expr TK_R_PAR */ yytestcase(yyruleno==42);
      /* (43) expr ::= expr OP_PLUS term */ yytestcase(yyruleno==43);
      /* (44) expr ::= expr OP_MINUS term */ yytestcase(yyruleno==44);
      /* (45) expr ::= term */ yytestcase(yyruleno==45);
      /* (46) term ::= term OP_MUL ssj_term */ yytestcase(yyruleno==46);
      /* (47) term ::= term OP_DIV ssj_term */ yytestcase(yyruleno==47);
      /* (48) term ::= term OP_MOD ssj_term */ yytestcase(yyruleno==48);
      /* (49) term ::= ssj_term */ yytestcase(yyruleno==49);
      /* (50) ssj_term ::= ssj_term OP_POW factor */ yytestcase(yyruleno==50);
      /* (51) ssj_term ::= factor */ yytestcase(yyruleno==51);
      /* (52) factor ::= TK_ID */ yytestcase(yyruleno==52);
      /* (53) factor ::= TK_ID TK_DOT TK_ID */ yytestcase(yyruleno==53);
      /* (54) factor ::= literal */ yytestcase(yyruleno==54);
      /* (55) factor ::= TK_L_PAR expr TK_R_PAR */ yytestcase(yyruleno==55);
      /* (56) factor ::= KW_TYPEOF TK_ID */ yytestcase(yyruleno==56);
      /* (57) factor ::= KW_TYPEOF TK_ID TK_DOT TK_ID */ yytestcase(yyruleno==57);
      /* (58) literal ::= LIT_INT */ yytestcase(yyruleno==58);
      /* (59) literal ::= LIT_BOOL */ yytestcase(yyruleno==59);
      /* (60) literal ::= LIT_CHAR */ yytestcase(yyruleno==60);
      /* (61) literal ::= LIT_DOUBLE */ yytestcase(yyruleno==61);
      /* (62) literal ::= LIT_STRING */ yytestcase(yyruleno==62);
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
#line 17 "grammar.y"

    cout<<"Error at: "<<yylineno<<endl;
#line 1031 "expression_parser.cpp"
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
#line 13 "grammar.y"

    cout<<"Done!"<<endl;
#line 1055 "expression_parser.cpp"
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
