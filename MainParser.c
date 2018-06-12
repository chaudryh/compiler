/*

File Name: MainParser
Course: Compiler Theory
Project Title: Variables
Author: Hasana Chaudry
Date: March 9th, 2017
Description: An advancement of the expression parser

*/

void program();
void external_definition();
void function_definition();
void parameter_list();
void data_definition(struct symbol_t *s);
void data_definition2();
void declaration_list();
void compound_statement();
void statement_list();
void statement();
void expression_statement();
void if_statement();
void while_statement();
void do_statement();
void continue_statement();
void break_statement();
void return_statement();

#include "atto-C.h"

int lookahead;  // The lookahead token

int local_offset;

int param_offset = 8;


void program()
{
	lookahead = lexer();
	while (lookahead != EOF_TOK)
		external_definition();
}
void external_definition()
{
	printf("Entering external defintion.\n");
	struct symbol_t *s;

	if (lookahead != INT_TOK)
	{
		printf("Missing type specifier for external definition. Line %d.\n", line_no);
		exit(0);
	}
	lookahead = lexer();  // eat left curly brace
	if (lookahead != STRING_TOK)
	{
		printf("Missing identifier. Line %d.\n", line_no);
		exit(1);
	}
	s = push_symbol(lexeme);
	lookahead = lexer();  // eat left curly brace
	if (lookahead == LEFT_PAREN_TOK)
	{
		function_definition();
	}
	else {
		data_definition(s);
	}
	printf("Leaving external defintion.\n");

}
void function_definition()
{
	fprintf(asmfile, "\n_TEXT    SEGMENT\n");

	printf("Entering function definition.\n");
	lookahead = lexer();  // eat left paren
	param_offset = 8;
	parameter_list();

	if (lookahead != RIGHT_PAREN_TOK)
	{
		printf("Missing right paren for function definition. Line %d.\n", line_no);
		exit(2);
	}
	lookahead = lexer();  // eat right paren
	local_offset = -4;
	compound_statement();

	fprintf(asmfile, "_TEXT    ENDS\n");

	printf("Leaving function definition.\n");

}
void parameter_list()
{
	struct symbol_t *ss;
	
	if (lookahead == RIGHT_PAREN_TOK)
	{
		return;
	}
	printf("Entering parameter list.\n");
	if (lookahead != INT_TOK)
	{
		printf("Missing type specifier for parameter list. Line %d.\n", line_no);
		exit(3);
	}
	lookahead = lexer();  // eat left curly brace
	if (lookahead != STRING_TOK)
	{
		printf("Missing identifier. Line %d.\n", line_no);
		exit(4);
	}
	ss = push_symbol(lexeme);
	sprintf(ss->asm_name, "_%s$[ebp]", ss->name);
	ss->symbol_kind = FUNCTION_PARAMETER;
	fprintf(asmfile, "_%s$ = %d\n", ss->name, param_offset);
	param_offset += 4;

	lookahead = lexer();  // eat identifier
	if (lookahead == COMMA_TOK)
	{
		lookahead = lexer();  // eat comma
		parameter_list();
	}
	if (lookahead == RIGHT_PAREN_TOK)
	{
		return;
	}
	else {
		printf("Error in parameter list. Line %d.\n", line_no);
		exit(5);
	}	
	printf("Leaving parameter list.\n");

}
void data_definition(struct symbol_t *s)
{
	printf("Entering data defintion.\n");

	fprintf(asmfile, "\n_DATA    SEGMENT\n");

	s->asm_name[0] = '_';
	strncpy(s->asm_name + 1, s->name, IDENT_MAX_LENGTH);

	fprintf(asmfile, "COMM  %s:DWORD\n", s->asm_name);

	s->symbol_kind = GLOBAL_VARIABLE;

	struct symbol_t *ss;

	while (lookahead == COMMA_TOK)
	{
		lookahead = lexer();
		if (lookahead != STRING_TOK) {
			printf("Missing identifier.\n");
			exit(6);
		}
		ss = push_symbol(lexeme);
		ss->asm_name[0] = '_';
		strncpy(ss->asm_name + 1, ss->name, IDENT_MAX_LENGTH);
		fprintf(asmfile, "COMM  %s:DWORD\n", ss->asm_name);
		ss->symbol_kind = GLOBAL_VARIABLE;

		lookahead = lexer();
	}
	if (lookahead != SEMI_COLON_TOK)
	{
		printf("Missing semi colon in data definition. Line %d.\n", line_no);
		exit(7);
	}
	lookahead = lexer();  // eat semi colon

	fprintf(asmfile, "_DATA    ENDS\n");

	printf("Leaving data defintion.\n");
}
void data_definition2()
{
	printf("Entering data defintion 2.\n");
	struct symbol_t *ss;	

	if (lookahead != INT_TOK)
	{
		printf("Missing type specifier.");
	}
	lookahead = lexer();
	if (lookahead != STRING_TOK)
	{
		printf("Missing identifier for declarator list. Line %d.\n", line_no);
		exit(8);
	}
	ss = push_symbol(lexeme);
	sprintf(ss->asm_name, "_%s$%d[ebp]", ss->name, local_offset / (-4));
	ss->symbol_kind = LOCAL_VARIABLE;
	fprintf(asmfile, "_%s$%d = %d\n", ss->name, local_offset / (-4), local_offset);
	local_offset -= 4;
	lookahead = lexer();  // eat identifier
	while (lookahead == COMMA_TOK)
	{
		lookahead = lexer();  // eat comma
		if (lookahead != STRING_TOK)
		{
			printf("Missing identifier in declarator list. Line %d.\n", line_no);
			exit(9);
		}
		ss = push_symbol(lexeme);
		sprintf(ss->asm_name, "_%s$%d[ebp]", ss->name, local_offset / (-4));
		ss->symbol_kind = LOCAL_VARIABLE;
		fprintf(asmfile, "_%s$%d = %d\n", ss->name, local_offset / (-4), local_offset);
		local_offset -= 4;
		lookahead = lexer();  // eat identifier
	}
	if (lookahead != SEMI_COLON_TOK)
	{
		printf("Missing semi colon for declarator list. Line %d.\n", line_no);
		exit(10);
	}

	lookahead = lexer();  // eat semi colon
	printf("Leaving data defintion 2.\n");
}
void compound_statement()
{
	printf("Entering compound statement.\n");
	if (lookahead != LEFT_CURLY_TOK)
	{
		printf("Missing left curly brace for compound statement. Line %d. \n", line_no);
		exit(11);
	}
	lookahead = lexer();  // eat left curly brace
	declaration_list();
	statement_list();

	if (lookahead != RIGHT_CURLY_TOK)
	{
		printf("Missing right curly brace at end of compound statement. Line %d.\n", line_no);
		exit(12);
	}
	lookahead = lexer();  // eat left curly brace   
	printf("Leaving compound statement.\n");
}
void statement_list()
{
	printf("Entering statement list.\n");

	while (lookahead != RIGHT_CURLY_TOK)
	{
		statement();
	}
	printf("Leaving statement list.\n");

}
void statement()
{
	printf("Entering statement.\n");
	//lookahead = lexer();
	if (lookahead == IF_TOK)
	{
		if_statement();
	}
	else if (lookahead == LEFT_CURLY_TOK)
	{
		compound_statement();
	}
	else if (lookahead == STRING_TOK || lookahead == INTEGER_TOK || 
		lookahead == MINUS_TOK || lookahead == EXCL_MARK_TOK || 
		lookahead == LEFT_PAREN_TOK || lookahead == EQUAL_TOK )
	{
		expression_statement();
	}
	else if (lookahead == WHILE_TOK)
	{
		while_statement();
	}
	else if (lookahead == DO_TOK)
	{
		do_statement();
	}
	else if (lookahead == CONT_TOK)
	{
		continue_statement();
	}
	else if (lookahead == BREAK_TOK)
	{
		break_statement();
	}
	else if (lookahead == RETURN_TOK)
	{
		return_statement();
	}
	else
	{
		printf("You done effed up, bruhhh. Line %d.\n", line_no);
		exit(13);
	}
	printf("Leaving statement.\n");

}
void expression_statement()
{
	printf("Entering expression statement.\n");
	expr_parser();
	if (lookahead != SEMI_COLON_TOK)
	{	
		printf("Missing semi colon for expression statement. Line %d.\n", line_no);
		exit(14);
	}
	lookahead = lexer();  // eat semi colon
	printf("Leaving expression statement.\n");

}
void if_statement()
{
	printf("Entering if statement.\n");
	lookahead = lexer();
	if (lookahead != LEFT_PAREN_TOK)
	{
		printf("Missing left paren for opening if statement. Line %d.\n", line_no);
		exit(15);
	}
	lookahead = lexer();  // eat left paren
	expr_parser();
	if (lookahead != RIGHT_PAREN_TOK)
	{
		printf("Missing right paren at end of if statement. Line %d.\n", line_no);
		exit(16);
	}
	lookahead = lexer();  // eat right paren
	statement();
	if (lookahead == ELSE_TOK)
	{
		lookahead = lexer();  // eat right paren
		statement();
	}
	printf("Leaving if statement.\n");

}
void while_statement()
{
	printf("Entering while statement.\n");
	lookahead = lexer();
	if (lookahead != LEFT_PAREN_TOK)
	{
		printf("Missing left paren for opening while statement. Line %d.\n", line_no);
		exit(17);
	}
	lookahead = lexer();  // eat left curly brace
	expr_parser();

	if (lookahead != RIGHT_PAREN_TOK)
	{
		printf("Missing right paren at end of while statement. Line %d.\n", line_no);
		exit(18);
	}
	lookahead = lexer();  // eat left curly brace 
	statement();
	printf("Leaving while statement.\n");

}
void do_statement()
{
	printf("Entering do statement.\n");
	lookahead = lexer(); // eat the do
	statement();
	if(lookahead != WHILE_TOK)
	{
		printf("Missinng while in do statement. Line %d \n", line_no);
		exit(19);
	}
	lookahead = lexer();
	if (lookahead != LEFT_PAREN_TOK)
	{
		printf("Missing left paren for opening while statement in do. Line %d.\n", line_no);
		exit(20);
	}
	lookahead = lexer();  // eat left curly brace
	expr_parser();

	if (lookahead != RIGHT_PAREN_TOK)
	{
		printf("Missing right paren at end of while statement. Line %d.\n", line_no);
		exit(21);
	}
	lookahead = lexer();  // eat lright paren
	if (lookahead != SEMI_COLON_TOK)
	{
		printf("Missing semi colon at end of do statement. Line %d.\n", line_no);
		exit(22);
	}
	lookahead = lexer();  // eat left curly brace 
	printf("Leaving do statement.\n");

}
void continue_statement()
{
	printf("Entering continue statement.\n");
	lookahead = lexer();
	if (lookahead != SEMI_COLON_TOK)
	{
		printf("Missing semi colon at end of continue statement. Line %d.\n", line_no);
		exit(23);
	}
	lookahead = lexer();  // eat left curly brace    
	printf("Leaving continue statement.\n");

}
void break_statement()
{
	printf("Entering break statement.\n");
	lookahead = lexer();
	if (lookahead != SEMI_COLON_TOK)
	{
		printf("Missing semi colon at end of break statement. Line %d. \n", line_no);
		exit(24);
	}
	lookahead = lexer();  // eat semi colon
	printf("Leaving break statement.\n");

}
void return_statement()
{
	printf("Entering return statement.\n");
	lookahead = lexer();
	expr_parser();
	if (lookahead != SEMI_COLON_TOK)
	{
		printf("Missing semi colon at end of return statement. Line %d. \n", line_no);
		exit(25);
	}
	lookahead = lexer();  // eat left curly brace   
	printf("Leaving return statement.\n");

}
void declaration_list()
{
	printf("Entering declaration list.\n");
	while (lookahead == INT_TOK)
	{
		data_definition2();
	}
	printf("Leaving declaration list.\n");

}

