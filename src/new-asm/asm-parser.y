%code top {
#include <cstdio>
#include "asm-lexer.h"

void yyerror (YYLTYPE *locp, asm_program *& program, char const *);
}

%code requires {
#include "parser_definitions.h"
#include "asm-classes.h"
#include "asm-program.h"
#include "asm-function.h"

int yyparse(asm_program *& program);
}

%output "asm-parser.cpp"
%defines "asm-parser.h"
%locations
%define api.pure
%error-verbose
%parse-param {asm_program *& program}

%union {
  char *string;
  int integer;
  float real;
  int instruction;
  char *id;
  char *label;

  struct asm_arg *arg;
  struct asm_statement *stmt;
  struct asm_instruction_statement *instr_stmt;
  struct asm_data_statement *data_stmt;
  struct asm_data_keyword_statement *keyw_stmt;
  struct asm_function *function;

  list<asm_function *>  * listOfFunctions;
  list<asm_statement *>  * listOfStatements;
  list<asm_data_statement *>  * listOfDataStatements;
}

%token <integer> INTEGER
%token <integer> CONTENT_CONST
%token <real> REAL
%token <string> STRING
%token <instruction> INSTRUCTION
%token KEYWORD_INT KEYWORD_LONG KEYWORD_REAL KEYWORD_CHAR KEYWORD_STRING
%token KEYWORD_LOCAL KEYWORD_GLOBAL KEYWORD_FUNCTION KEYWORD_END
%token <label> DEF_LABEL
%token <label> POSITION_LABEL
%token <label> CONTENT_LABEL
%token <arg> REGISTER
%token <id> ID
%token COMA
%token END_LINE

%type <arg> arg
%type <stmt> stmt
%type <listOfStatements> stmts
%type <instr_stmt> instruction_stmt
%type <data_stmt> data_stmt
%type <keyw_stmt> data_keyword
%type <listOfDataStatements> data_stmts
%type <listOfDataStatements> locals
%type <listOfDataStatements> globals
%type <function> function
%type <listOfFunctions> functions

%%

program
      : functions
          { program = new asm_program( $1, new list<asm_data_statement *>() ) }
      | globals blank_lines functions
          { program = new asm_program( $3, $1 ) }
      | blank_lines functions
          { program = new asm_program( $2, new list<asm_data_statement *>() ) }
      | blank_lines globals blank_lines functions
          { program = new asm_program( $4, $2 ) }
      | functions blank_lines
          { program = new asm_program( $1, new list<asm_data_statement *>() ) }
      | globals blank_lines functions blank_lines
          { program = new asm_program( $3, $1 ) }
      | blank_lines functions blank_lines
          { program = new asm_program( $2, new list<asm_data_statement *>() ) }
      | blank_lines globals blank_lines functions blank_lines
          { program = new asm_program( $4, $2 ) }
      ;

globals
      : KEYWORD_GLOBAL blank_lines data_stmts blank_lines KEYWORD_END
                  { $$ = $3 }
      | KEYWORD_GLOBAL blank_lines KEYWORD_END
                  { $$ = new list<asm_data_statement *>() }
      ;

functions
      : functions blank_lines function
                  { $1->push_back( $3 ); $$ = $1 }
      | function  { $$ = new list<asm_function *>();
                    $$->push_back( $1 ) }
      ;

blank_lines
      : blank_lines END_LINE
      | END_LINE
      ;

function
      : KEYWORD_FUNCTION STRING blank_lines
        locals blank_lines
        stmts blank_lines
        KEYWORD_END
                  { $$ = new asm_function( $2, $6, $4 ) }
      ;

locals
      : KEYWORD_LOCAL blank_lines data_stmts blank_lines KEYWORD_END
                  { $$ = $3 }
      | KEYWORD_LOCAL blank_lines KEYWORD_END
                  { $$ = new list<asm_data_statement *>() }
      ;

stmts
      : stmts blank_lines stmt
                  { $1->push_back( $3 ); $$ = $1 }
      | stmt      { $$ = new list<asm_statement *>();
                    $$->push_back( $1 ) }
      ;

arg
      : INTEGER         { $$ = new asm_immediate_arg( yylloc, $1 , COST ) }
      | CONTENT_CONST   { $$ = new asm_immediate_arg( yylloc, $1 , ADDR ) }
      | REAL            { $$ = new asm_immediate_arg( yylloc, $1 ) }
      | POSITION_LABEL  { $$ = new asm_label_arg( yylloc, $1 , COST ) }
      | CONTENT_LABEL   { $$ = new asm_label_arg( yylloc, $1 , ADDR ) }
      | REGISTER        { $$ = $1 }
      ;

stmt
      : instruction_stmt  { $$ = $1 }
      | data_stmt         { $$ = $1 }
      ;

instruction_stmt
      : instruction_stmt arg  { $$ = $1->addArg( $2 ) }
      | INSTRUCTION           { $$ = new asm_instruction_statement( $1 ) }
      ;

data_stmts
      : data_stmts blank_lines data_stmt
                  { $1->push_back( $3 ); $$ = $1 }
      | data_stmt { $$ = new list<asm_data_statement *>();
                    $$->push_back( $1 ) }
      ;

data_stmt
      : DEF_LABEL     { $$ = new asm_label_statement( $1 ) }
      | data_keyword  { $$ = $1 }
      ;

data_keyword
      : KEYWORD_INT INTEGER   { $$ = new asm_int_keyword_statement( $2 ) }
      | KEYWORD_LONG INTEGER  { $$ = new asm_long_keyword_statement( $2 ) }
      | KEYWORD_REAL REAL     { $$ = new asm_real_keyword_statement( $2 ) }
      | KEYWORD_CHAR ID       { $$ = new asm_char_keyword_statement( $2 ) }
      | KEYWORD_STRING STRING { $$ = new asm_string_keyword_statement( $2 ) }
      ;


%%

void
yyerror (YYLTYPE *locp, asm_program *& program, char const *s) {
  fprintf(stderr, "ASSEMBLER ERROR: %s\n - at line: %4d col: %4d\n", s,
          locp->first_line, locp->first_column);
}


