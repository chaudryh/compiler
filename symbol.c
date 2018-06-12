/*

File Name: symbol.c
Course: Compiler Theory
Project Title: Variables
Author: Hasana Chaudry
Date: April 18th, 2017
Description: An advancement of the expression parser

*/

#include "atto-C.h"

struct symbol_t symbol_table[MAXSYMBOL];
int symbol_stack_top;

struct symbol_t *push_symbol(char *name)
{
	symbol_stack_top++;
	strcpy(symbol_table[symbol_stack_top].name, name);
	//check for stack overflow hereeeee
	if (stacktop >= MAXSYMBOL)
	{
		printf("Symbol stack overflow. \n");
		exit(0);
	}
	printf("Adding symbol '%s', table index %d, line %d\n", name,
		symbol_stack_top, line_no);
	return symbol_table + symbol_stack_top;

}

int start_scope()
{
	return symbol_stack_top;
}

void end_scope(int prev_stack_top)
{
	symbol_stack_top = prev_stack_top;
	/*error checking on input to make sure it is positive
	and no greater than the current stack top*/
	if (prev_stack_top < 0)
	{
		printf("Internal scope error. Line %d\n", line_no);
		exit(1);
	}
}

struct symbol_t *symbol_lookup(char *name)
{
	int k;

	printf("Looking up symbol '%s', table index %d, line %d\n", name,
		symbol_stack_top, line_no);

	for (k = symbol_stack_top; k > 0; k--)
	{
		if (strcmp(name, symbol_table[k].name) == 0)
			return symbol_table + k;
	}
	
	return NULL;
}

