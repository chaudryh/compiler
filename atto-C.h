/*


File Name: atto-C
Course: Compiler Theory
Project Title: Variables
Author: Hasana Chaudry
Date: Feb 9th, 2017
Description: A modification of the previous lexical analyzer for atto-C tokens


*/



#include <stdio.h>    // standard input-output declarations: printf, stdin
#include <stdlib.h>   // standard library declarations: exit
#include <ctype.h>    // character type test declarations:  isdigit, isalpha, isalnum
#include <string.h>


#define INTEGER_TOK          101
#define DECIMAL_FRACTION_TOK 102
#define COMMENT_TOK		     103
#define COMMA_TOK			 104
#define	SEMI_COLON_TOK       105
#define EXCL_MARK_TOK        106
#define LESS_THAN_TOK        107
#define GREATER_THAN_TOK	 108
#define DIV_TOK	             109
#define EQUAL_TOK			 110
#define AMP_TOK				 111
#define LEFT_PAREN_TOK       112
#define RIGHT_PAREN_TOK      113
#define LEFT_CURLY_TOK       114
#define RIGHT_CURLY_TOK      115
#define MINUS_TOK            116
#define PLUS_TOK             117
#define NEW_LINE_TOK         118
#define TAB_TOK              119
#define SPACE_TOK            120
#define STRING_TOK           121
#define VERTICAL_TOK         122
#define MUL_TOK				 123
#define QUOTE_TOK            124
#define AUTO_TOK		     125
#define BREAK_TOK		     126
#define CASE_TOK		     127
#define CHAR_TOK		     128
#define CONT_TOK		     129
#define DEFAULT_TOK		     130
#define DO_TOK		         131
#define DOUBLE_TOK		     132
#define ELSE_TOK		     133
#define ENUM_TOK		     134
#define EXTERN_TOK		     135
#define FLOAT_TOK		     136
#define FOR_TOK		         137
#define GOTO_TOK		     138
#define IF_TOK		         139
#define INT_TOK		         140
#define REG_TOK		         141
#define RETURN_TOK		     142
#define SHORT_TOK		     143
#define SIZEOF_TOK		     144
#define STATIC_TOK		     145
#define STRUCT_TOK		     146
#define SWITCH_TOK		     147
#define TYPEDEF_TOK	     	 148
#define UNION_TOK		     149
#define UNSIGNED_TOK	 	 150
#define WHILE_TOK		     151
#define LONG_TOK			 152
#define EOF_TOK				 153
#define NOT_EQUAL_TOK		 154
#define GEQU_TOK			 155
#define LEQU_TOK			 156
#define DEQU_TOK			 157
#define DOLLAR_TOK			 158
#define UNARY_MINUS_TOK		 159



extern int line_no;  // line number in current input file

#define MAX_LEXEME 200

extern char lexeme[MAX_LEXEME]; // The characters of the token

#define IDENT_MAX_LENGTH 50

#define NUMBER_MAX_LENGTH 10

int lexer(void);

int expr_parser(void);

void program();

extern FILE *sourcefile;

extern FILE *asmfile;

void preamble(char *sourcefilename);

void postamble(void);

// Symbol classification
#define GLOBAL_VARIABLE    1
#define LOCAL_VARIABLE     2
#define FUNCTION_PARAMETER 3
#define FUNCTION           4

// Symbol table declarations
struct symbol_t {
	char name[IDENT_MAX_LENGTH + 1];
	char asm_name[IDENT_MAX_LENGTH + 1];
	int symbol_kind;
};
#define MAXSYMBOL 1000
extern struct symbol_t symbol_table[MAXSYMBOL];
extern int symbol_stack_top;

struct symbol_t *push_symbol(char *name);

int start_scope(void);

void end_scope(int prev_stack_star);

struct symbol_t *symbol_lookup(char *name);


extern int lookahead;
extern int stacktop;
#define STACKMAX 1000
