/*

File Name: ExprParser
Course: Compiler Theory
Project Title: Variables
Author: Hasana Chaudry
Date: March 1st, 2017
Description: An expression parser

*/

#include "atto-C.h"

int f(int token_type)
{
	switch (token_type)
	{
	case INTEGER_TOK: return 100;
	case COMMA_TOK: return 10;
	case SEMI_COLON_TOK: return 5;
	case EXCL_MARK_TOK: return 106;
	case LESS_THAN_TOK: return 60;
	case GREATER_THAN_TOK: return 60;
	case DIV_TOK: return 80;
	case EQUAL_TOK: return 20;
	case AMP_TOK: return 53;
	case LEFT_PAREN_TOK: return 5;
	case RIGHT_PAREN_TOK: return 8;
	case MINUS_TOK: return 75;
	case PLUS_TOK: return 75;
	case STRING_TOK: return 90;
	case VERTICAL_TOK: return 51;
	case MUL_TOK: return 80;
	case NOT_EQUAL_TOK: return 60;
	case GEQU_TOK: return 60;
	case LEQU_TOK: return 60;
	case DEQU_TOK: return 60;
	case DOLLAR_TOK: return 0;
	case UNARY_MINUS_TOK: return 55;
	default: printf("Expression parser f: Illegal token type: %d\n", token_type);
		exit(1);
	}
}

int g(int token_type)
{
	switch (token_type)
	{
	case INTEGER_TOK: return 90;
	case COMMA_TOK: return 89;
	case SEMI_COLON_TOK: return 0;
	case EXCL_MARK_TOK: return 106;
	case LESS_THAN_TOK: return 55;
	case GREATER_THAN_TOK: return 55;
	case DIV_TOK: return 79;
	case EQUAL_TOK: return 79;
	case AMP_TOK: return 52;
	case LEFT_PAREN_TOK: return 95;
	case RIGHT_PAREN_TOK: return 5;
	case MINUS_TOK: return 70;
	case PLUS_TOK: return 70;
	case STRING_TOK: return 107; //was 85
	case VERTICAL_TOK: return 50;
	case MUL_TOK: return 79;
	case EOF_TOK: return 0;
	case NOT_EQUAL_TOK: return 55;
	case GEQU_TOK: return 55;
	case LEQU_TOK: return 55;
	case DEQU_TOK: return 55;
	case DOLLAR_TOK: return 0;
	case UNARY_MINUS_TOK: return 54;
	default: printf("Expression parser g: Illegal token type: %d\n", token_type);
		exit(1);
	}
}

struct stack_t {
	int token;  // type of token in this stack position
	int expr_after;  // 1 for nonterminal following, 0 for not.

};
#define STACKMAX 1000
struct stack_t stack[STACKMAX];
int stacktop;   // index of the current top of the stack

/*
This is the most important function in the parser. It goes goes
through the input and decides what action to take (shift/reduce).
It uses an infinite loop to go through the parsing part. The unary
minus is dealt with seperately. The input is either rejected or
parsed and a print statement is displayed for both. The parser
also detects errors in the format of the input expressions.
*/


int lookahead;

int expr_parser()
{
	// initialize stack

	stack[0].token = DOLLAR_TOK;
	stack[0].expr_after = 0;
	stacktop = 0;
	if (lookahead == 0)
	{
		lookahead = lexer();
	}


	for (;;) {
		if (lookahead == MINUS_TOK && !stack[stacktop].expr_after)
			lookahead = UNARY_MINUS_TOK;
		if (lookahead == UNARY_MINUS_TOK && stack[stacktop].expr_after) {
			lookahead = MINUS_TOK;
		}

		if (f(stack[stacktop].token) <= g(lookahead))
		{
			if (stacktop == 0 && ((g(lookahead) == 0) || (lookahead== RIGHT_PAREN_TOK))) {
				printf("Accept expression \n");
				return lookahead;
			}
			printf("Shift \n");
			stacktop++;
			if (stacktop >= STACKMAX)
			{
				printf("Stack overflow. \n");
			}
			stack[stacktop].token = lookahead;
			stack[stacktop].expr_after = 0;
			if (lookahead == STRING_TOK)
				symbol_lookup(lexeme);
			lookahead = lexer();
		}
		else
		{
			printf("Reduce \n");
			switch (stack[stacktop].token)
			{
			case INTEGER_TOK:
				if (stack[stacktop].expr_after)
				{
					printf("INTEGER with following expression, line %d\n", line_no); exit(1);
				}
				if (stack[stacktop - 1].expr_after)
				{
					printf("INTEGER with preceding expression, line %d\n", line_no); exit(1);
				}
				stacktop--; stack[stacktop].expr_after = 1; break;
			case COMMA_TOK:
				if (!stack[stacktop].expr_after)
				{
					printf("COMMA without following expression, line %d\n", line_no); exit(1);
				}
				if (!stack[stacktop - 1].expr_after)
				{
					printf("COMMA without preceding expression, line %d\n", line_no); exit(1);
				}
				stacktop--; stack[stacktop].expr_after = 1; break;
			case SEMI_COLON_TOK:
				if (!stack[stacktop].expr_after)
				{
					printf("SEMI COLON without following expression, line %d\n", line_no); exit(1);
				}
				if (!stack[stacktop - 1].expr_after)
				{
					printf("SEMI COLON without preceding expression, line %d\n", line_no); exit(1);
				}
				stacktop--; stack[stacktop].expr_after = 1; break;
			case EXCL_MARK_TOK:
				if (!stack[stacktop].expr_after)
				{
					printf("EXCLAMATION MARK without following expression, line %d\n", line_no); exit(1);
				}
				stacktop--; stack[stacktop].expr_after = 1; break;
			case LESS_THAN_TOK:
				if (!stack[stacktop].expr_after)
				{
					printf("LESS THAN without following expression, line %d\n", line_no); exit(1);
				}
				if (!stack[stacktop - 1].expr_after)
				{
					printf("LESS THAN without preceding expression, line %d\n", line_no); exit(1);
				}
				stacktop--; stack[stacktop].expr_after = 1; break;
			case GREATER_THAN_TOK:
				if (!stack[stacktop].expr_after)
				{
					printf("GREATER THAN without following expression, line %d\n", line_no); exit(1);
				}
				if (!stack[stacktop - 1].expr_after)
				{
					printf("GREATER THAN without preceding expression, line %d\n", line_no); exit(1);
				}
				stacktop--; stack[stacktop].expr_after = 1; break;
			case DIV_TOK:
				if (!stack[stacktop].expr_after)
				{
					printf("DIVISION without following expression, line %d\n", line_no); exit(1);
				}
				if (!stack[stacktop - 1].expr_after)
				{
					printf("DIVISION without preceding expression, line %d\n", line_no); exit(1);
				}
				stacktop--; stack[stacktop].expr_after = 1; break;
			case EQUAL_TOK:
				if (!stack[stacktop].expr_after)
				{
					printf("EQUAL without following expression, line %d\n", line_no); exit(1);
				}
				if (!stack[stacktop - 1].expr_after)
				{
					printf("EQUAL without preceding expression, line %d\n", line_no); exit(1);
				}
				stacktop--; stack[stacktop].expr_after = 1; break;
			case AMP_TOK:
				if (!stack[stacktop].expr_after)
				{
					printf("AMPERSAND without following expression, line %d\n", line_no); exit(1);
				}
				if (!stack[stacktop - 1].expr_after)
				{
					printf("AMPERSAND without preceding expression, line %d\n", line_no); exit(1);
				}
				stacktop--; stack[stacktop].expr_after = 1; break;
			case LEFT_PAREN_TOK:
				if (!stack[stacktop].expr_after)
				{
					printf("LEFT PAREN without following expression, line %d\n", line_no); exit(1);
				}
				if (!stack[stacktop - 1].expr_after)
				{
					printf("LEFT PAREN without preceding expression, line %d\n", line_no); exit(1);
				}
				stacktop--; stack[stacktop].expr_after = 1; break;
			case RIGHT_PAREN_TOK:
				if (!stack[stacktop].expr_after)
				{
					printf("RIGHT PAREN without following expression, line %d\n", line_no); exit(1);
				}
				if (!stack[stacktop - 1].expr_after)
				{
					printf("RIGHT PAREN without preceding expression, line %d\n", line_no); exit(1);
				}
				stacktop--; stack[stacktop].expr_after = 1; break;
			case MINUS_TOK:
				if (!stack[stacktop].expr_after)
				{
					printf("MINUS without following expression, line %d\n", line_no); exit(1);
				}
				if (!stack[stacktop - 1].expr_after)
				{
					printf("MIUS without preceding expression, line %d\n", line_no); exit(1);
				}
				stacktop--; stack[stacktop].expr_after = 1; break;
			case PLUS_TOK:
				if (!stack[stacktop].expr_after)
				{
					printf("PLUS without following expression, line %d\n", line_no); exit(1);
				}
				if (!stack[stacktop - 1].expr_after)
				{
					printf("PLUS without preceding expression, line %d\n", line_no); exit(1);
				}
				stacktop--; stack[stacktop].expr_after = 1; break;
			case STRING_TOK:
				if (stack[stacktop].expr_after)
				{
					printf("STRING with following expression, line %d\n", line_no); exit(1);
				}
				if (stack[stacktop - 1].expr_after)
				{
					printf("STRING with preceding expression, line %d\n", line_no); exit(1);
				}
				stacktop--; stack[stacktop].expr_after = 1; break;
			case VERTICAL_TOK:
				if (!stack[stacktop].expr_after)
				{
					printf("VERTICAL BAR without following expression, line %d\n", line_no); exit(1);
				}
				if (!stack[stacktop - 1].expr_after)
				{
					printf("VERTICAL BAR without preceding expression, line %d\n", line_no); exit(1);
				}
				stacktop--; stack[stacktop].expr_after = 1; break;
			case MUL_TOK:
				if (!stack[stacktop].expr_after)
				{
					printf("MULTIPLICATION without following expression, line %d\n", line_no); exit(1);
				}
				if (!stack[stacktop - 1].expr_after)
				{
					printf("MULTIPLICATION without preceding expression, line %d\n", line_no); exit(1);
				}
				stacktop--; stack[stacktop].expr_after = 1; break;
			case NOT_EQUAL_TOK:
				if (!stack[stacktop].expr_after)
				{
					printf("NOT EQUAL without following expression, line %d\n", line_no); exit(1);
				}
				if (!stack[stacktop - 1].expr_after)
				{
					printf("NOT EQUAL without preceding expression, line %d\n", line_no); exit(1);
				}
				stacktop--; stack[stacktop].expr_after = 1; break;
			case LEQU_TOK:
				if (!stack[stacktop].expr_after)
				{
					printf("LESS THAN OR EQUAL TO without following expression, line %d\n", line_no); exit(1);
				}
				if (!stack[stacktop - 1].expr_after)
				{
					printf("LESS THAN OR EQUAL TO without preceding expression, line %d\n", line_no); exit(1);
				}
				stacktop--; stack[stacktop].expr_after = 1; break;
			case GEQU_TOK:
				if (!stack[stacktop].expr_after)
				{
					printf("GREATER THAN OR EQUAL TO without following expression, line %d\n", line_no); exit(1);
				}
				if (!stack[stacktop - 1].expr_after)
				{
					printf("GREATER THAN OR EQUAL TO without preceding expression, line %d\n", line_no); exit(1);
				}
				stacktop--; stack[stacktop].expr_after = 1; break;
			case DEQU_TOK:
				if (!stack[stacktop].expr_after)
				{
					printf("DOUBLE EQUAL without following expression, line %d\n", line_no); exit(1);
				}
				if (!stack[stacktop - 1].expr_after)
				{
					printf("DOUBLE EQUAL without preceding expression, line %d\n", line_no); exit(1);
				}
				stacktop--; stack[stacktop].expr_after = 1; break;
			case DOLLAR_TOK:
				if (!stack[stacktop].expr_after)
				{
					printf("DOLLAR SIGN without following expression, line %d\n", line_no); exit(1);
				}
				if (!stack[stacktop - 1].expr_after)
				{
					printf("DOLLAR SIGN without preceding expression, line %d\n", line_no); exit(1);
				}
				stacktop--; stack[stacktop].expr_after = 1; break;
			case UNARY_MINUS_TOK:
				if (!stack[stacktop].expr_after)
				{
					printf("UNIARY MINUS without following expression, line %d\n", line_no); exit(1);
				}
				if (stack[stacktop - 1].expr_after)
				{
					printf("UNIARY with preceding expression, line %d\n", line_no); exit(1);
				}
				stacktop--; stack[stacktop].expr_after = 1; break;
			default:
				printf("Bad stack top token %d\n", stack[stacktop].token);
				exit(2);
			}
		}
	}
}