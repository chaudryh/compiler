/*lexer.c


	Course: Compiler Theory
	Project Title: Lexer2
	Author: Hasana Chaudry
	Date: Feb 9th, 2017
	Description: A modification of the previous lexical analyzer for atto-C tokens


   An advanced version of the previous lexical analyzer.
   A Finite State machine is used to recognize strings consisting
   of digits followed by a decimal point followed by digits.
   There must be at least one digit before the decimal point.
   The decimal point and digits after the decimal point are optional. 
   The program is designed to rcognize tokens, comment strings, quote 
   strings as well as a tab, newline, quote, or backspace within a
   quote string.

*/

 #include "atto-C.h"


 // Finite State Machine states
 #define START  1
 #define NUM1   2
 #define NUM2   3
 #define CMT1   4
 #define CMT2   5
 #define CMT3   6
 #define CMT4   7
 #define STRING 8
 #define GEQU   9
 #define LEQU   10
 #define NEQU   11
 #define EQUAL  12
 #define DVERT  13
 #define DAMP   14
 #define QUOTE  15
 #define FINAL  16
 #define BACKSLASH_IN_QUOTE 17

#define KEYWORDS_MAX  28

char *keywords[KEYWORDS_MAX] = { "auto", "break", "case", "char", 
"continue", "default", "do", "double", "else", "enum", "extern", 
"float", "for", "goto", "if", "int", "long", "register", "return", 
"short", "sizeof", "static", "struct", "switch", "typedef", "union", 
"unsigned", "while" };

int keyword_tokens[KEYWORDS_MAX] = { AUTO_TOK, BREAK_TOK, CASE_TOK, 
CHAR_TOK, CONT_TOK, DEFAULT_TOK, DO_TOK, DOUBLE_TOK, ELSE_TOK, ENUM_TOK, EXTERN_TOK,
FLOAT_TOK, FOR_TOK, GOTO_TOK, IF_TOK, INT_TOK, LONG_TOK, REG_TOK, 
RETURN_TOK, SHORT_TOK, SIZEOF_TOK, STATIC_TOK, STRUCT_TOK, SWITCH_TOK, 
TYPEDEF_TOK, UNION_TOK, UNSIGNED_TOK, WHILE_TOK };

int line_no = 1;  // line number in current input file
char lexeme[MAX_LEXEME]; // The characters of the token

typedef int boolean;
  #define  true              1
  #define false              0

int next_char; // The next character of input.

int lexer()
 { int state;   // The current state of the FSM.
   int lex_spot; // Current spot in lexeme.
   int token_type;  // The type of token found.
   boolean check;

     // Infinite loop, doing one token at a time.
     while ( 1 )
     {  // Initialize the Finite State Machine.
        state = START;
        lex_spot = 0;
        check  = false;
        // Loop over characters of the token.
        while ( state != FINAL )
        { if ( next_char == 0 )
             next_char = getc(sourcefile);  // get one character from standard input
          
          switch ( state )
          { case START:
			  if (next_char == EOF)      // EOF is special character for End-Of-File
				  return EOF_TOK;
			  if (next_char == '\n')  // just eat the newline and stay in START
			  {
				  line_no++;
				  next_char = 0;
			  }
                else if ( isdigit(next_char) )
                { state = NUM1;
                  lexeme[lex_spot++] = next_char;  // Add the character to the lexeme
                  next_char = 0;  // eat the character
                }else if(next_char == '/'){
                    state = CMT1;
                    lexeme[lex_spot++] = next_char;
                    next_char = 0;
                }else if(isalpha(next_char)|next_char == '_'){
                    if(next_char == '_'){
                       check = true;
                    }
                    state = STRING;
                    lexeme[lex_spot++] = next_char;
                    next_char = 0;
				}
				else if (next_char == '+' || next_char == '-' || next_char == '('
					|| next_char == ')' || next_char == '{' || next_char == '}' || next_char == ',' ||
					next_char == ';' || next_char == '*') {
					lexeme[lex_spot++] = next_char;
					lexeme[lex_spot] = 0;
	
					if (next_char == '+') {
						next_char = 0;
						lexeme[lex_spot] = 0;
						state = FINAL;
						token_type = PLUS_TOK;
						printf("ACCEPT PLUS %s \n", lexeme);
						return token_type;
						
					}
					else if (next_char == '-') {
						token_type = MINUS_TOK;
						next_char = 0;
						printf("ACCEPT MINUS %s \n", lexeme);
						return token_type;
						
					}
					else if (next_char == ';') {
						token_type = SEMI_COLON_TOK;
						next_char = 0;
						printf("ACCEPT SEMICOLON %s \n", lexeme);
						return token_type;
						
					}
					else if (next_char == ',') {
						token_type = COMMA_TOK;
						next_char = 0;
						printf("ACCEPT COMMA %s \n", lexeme);
						return token_type;
						
					}
					else if (next_char == '*') {
						token_type = MUL_TOK;
						next_char = 0;
						printf("ACCEPT MULTIPLICATION %s \n", lexeme);
						return token_type;
						
					}
					else if (next_char == '(') {
						token_type = LEFT_PAREN_TOK;
						next_char = 0;
						printf("ACCEPT LEFT_PAREN %s \n", lexeme);
						return token_type;
						
					}
					else if (next_char == ')') {
						token_type = RIGHT_PAREN_TOK;
						next_char = 0;
						printf("ACCEPT RIGHT_PAREN %s \n", lexeme);
						return token_type;
						
					}
					else if (next_char == '{') {
						token_type = LEFT_CURLY_TOK;
						next_char = 0;
						printf("ACCEPT LEFT_CURLY %s \n", lexeme);
						return token_type;
						
					}
					else if (next_char == '}') {
						token_type = RIGHT_CURLY_TOK;
						next_char = 0;
						printf("ACCEPT RIGHT_CURLY %s \n", lexeme);
						return token_type;
						
					}
					else {
						printf("ERROR. CAN'T HAPPEN. %s \n", lexeme);
						state = FINAL;
						next_char = 0;
						return token_type;
					}
                }else if(next_char=='!'){
                    state = NEQU;
                    lexeme[lex_spot++] = next_char;
                    next_char = 0;
                }else if(next_char == '"'){
                    state = QUOTE;
                    lexeme [lex_spot++] = next_char;
                    next_char = 0;
                }else if(next_char == '&'){
                    state = DAMP;
                    lexeme[lex_spot++] = next_char;
                    next_char = 0;
                }else if(next_char == ' '){
                    state = FINAL;
                    lexeme[lex_spot++] = next_char;
                    lexeme[lex_spot] = 0;
                    //printf("ACCEPT SPACE %s ",lexeme);
                    next_char = 0;
					token_type = SPACE_TOK;
					//return token_type;
					break;
                }else if(next_char == '>'){
                    state = GEQU;
                    lexeme[lex_spot++] = next_char;
                    next_char = 0;
                }else if(next_char == '<'){
                    state = LEQU;
                    lexeme[lex_spot++] = next_char;
                    next_char = 0;
                }else if(next_char == '='){
                    state = EQUAL;
                    lexeme[lex_spot++] = next_char;
                    next_char=0;
                }else if (next_char == '|'){
					
                    state = DVERT;
                    lexeme[lex_spot++] = next_char;
                    next_char = 0;
                }
                else
                { printf("REJECT %c Line number: %d\n",next_char,line_no);  // This is not a legal final state
                  state = FINAL;  // but we want to end the token anyway
                  next_char = 0;   // eat the offending character
                }
                break;  // Need "break" at the end of a case, else you will continue to the next case.

            case QUOTE:
				if (next_char == '"') {
					state = FINAL;
					lexeme[lex_spot++] = next_char;
					lexeme[lex_spot] = 0;
					next_char = 0;
					token_type = QUOTE_TOK;
					printf("ACCEPT QUOTE_STRING %s ", lexeme);
					return token_type;
				} 
				else if(next_char == '\\') {
					state = BACKSLASH_IN_QUOTE;
					next_char = 0;
				}
				else if (next_char == '\n') {
					state = FINAL;
					lexeme[lex_spot++] = 0;
					next_char = 0;
					printf("Error: Illegal newline in quote string. \n");
				}
				else {
					state = QUOTE;
					if (lex_spot == MAX_LEXEME - 3)
					{
						printf("ERROR: quoted string too long. Truncating. Line %d\n", line_no);
						lexeme[lex_spot++] = next_char;
					}
					else if (lex_spot < MAX_LEXEME - 3)
						lexeme[lex_spot++] = next_char;
					next_char = 0;
				}
				break;

			case BACKSLASH_IN_QUOTE:
				if (next_char == 'n') {
					lexeme[lex_spot++] = '\n';
					lexeme[lex_spot] = 0;
					next_char = 0;
					state = QUOTE;
				}
				if (next_char == 'b') {
					lexeme[lex_spot++] = '\b';
					lexeme[lex_spot] = 0;
					next_char = 0;
					state = QUOTE;
				}
				if (next_char == 't') {
					lexeme[lex_spot++] = '\t';
					lexeme[lex_spot] = 0;
					next_char = 0;
					state = QUOTE;
				}
				if (next_char == '"') {
					lexeme[lex_spot++] = '"';
					lexeme[lex_spot] = 0;
					next_char = 0;
					state = QUOTE;
				}
				if (next_char == '\\') {
					lexeme[lex_spot++] = '\\';
					lexeme[lex_spot] = 0;
					next_char = 0;
					state = QUOTE;
				}
				else {
					lexeme[lex_spot] = 0;
					state = QUOTE;
				}
				break;
	
            case DAMP:
                if(next_char == '&'){
                    lexeme[lex_spot++] = next_char;
                    lexeme[lex_spot] = 0;
                    next_char = 0;
                    state = FINAL;
					token_type = AMP_TOK;
                    printf("ACCEPT DOUBLE_AMPERSAND %s \n",lexeme);
					return token_type;
					
                }else{
                    state = FINAL;
					printf("REJECT %s Line number: %d\n", lexeme, line_no);

                }
                break;
            case DVERT:
               if(next_char =='|'){
                    lexeme[lex_spot++] = next_char;
                    lexeme[lex_spot]=0;
                    next_char = 0;
                    state = FINAL;
					token_type = VERTICAL_TOK;
                    printf("ACCEPT DOUBLE_VERTICAL %s \n",lexeme);
					return token_type;
				
               }else{
                    state = FINAL;
                    printf("REJECT %s Line number: %d\n",lexeme, line_no);
                }
                break;
            case EQUAL:
                if(next_char =='='){
                    lexeme[lex_spot++] = next_char;
                    lexeme[lex_spot]=0;
                    next_char = 0;
                    state = FINAL;
					token_type = DEQU_TOK;
                    printf("ACCEPT DOUBLE_EQUAL %s \n",lexeme);
					return token_type;
					
                }else{
                    lexeme[lex_spot]=0;
                    state = FINAL;
					token_type = EQUAL_TOK;
                    printf("ACCEPT EQUAL %s \n",lexeme);
					return token_type;
					
                }
                break;

            case LEQU:
                if(next_char =='='){
                    lexeme[lex_spot++] = next_char;
                    lexeme[lex_spot]=0;
                    next_char = 0;
                    state = FINAL;
					token_type = LEQU_TOK;
                    printf("ACCEPT LESS_THAN_OR_EQUAL %s \n",lexeme);
					return token_type;
					
                }else{
                    lexeme[lex_spot]=0;
                    state = FINAL;
					token_type = LESS_THAN_TOK;
                    printf("ACCEPT LESS_THAN %s \n",lexeme);
					return token_type;
					
                }
                break;
            case GEQU:
                if(next_char =='='){
                    lexeme[lex_spot++] = next_char;
                    lexeme[lex_spot]=0;
                    next_char = 0;
                    state = FINAL;
					token_type = GEQU_TOK;
                    printf("ACCEPT GREATER_THAN_OR_EQUAL %s \n",lexeme);
					return token_type;
					
                }else{
                    lexeme[lex_spot]=0;
                    state = FINAL;
					token_type = GREATER_THAN_TOK;
                    printf("ACCEPT GREATER_THAN %s \n",lexeme);
					return token_type;
					
                }
                break;

			case NEQU:
				if (next_char == '=') {
					state = FINAL;
					lexeme[lex_spot++] = next_char;
					lexeme[lex_spot] = 0;
					next_char = 0;
					token_type = NOT_EQUAL_TOK;
					printf("ACCEPT NOT_EQUAL %s \n", lexeme);
					return token_type;

				}
				else {
					state = FINAL;
					lexeme[lex_spot] = 0;
					token_type = EXCL_MARK_TOK;
					printf("ACCEPT EXCLAMATION_MARK %s ", lexeme);
					return token_type;
				}
				break;
            case STRING:
                if(isalpha(next_char)||isdigit(next_char)||next_char == '_'){
					state = STRING;
					if (lex_spot == IDENT_MAX_LENGTH)
					{
						printf("ERROR: string too long. Truncating. Line %d\n", line_no);
						lexeme[lex_spot++] = 0;
					}
					else if (lex_spot < IDENT_MAX_LENGTH)
						lexeme[lex_spot++] = next_char;
						next_char = 0;
                }
					
                else{
					int k;
                    lexeme[lex_spot] = 0;
					token_type = STRING_TOK;
                    state = FINAL;
					for (k = 0; k < KEYWORDS_MAX; k++)
					{
						if (strcmp(lexeme, keywords[k]) == 0)
						{
							token_type = keyword_tokens[k];
							break;
						}
					}
					printf("ACCEPT STRING %s \n", lexeme);
					return token_type;
                }
                break;
            case CMT1:
                if(next_char == '/' ){
                    state = CMT4;
                    lexeme[lex_spot++] = next_char;
                    next_char = 0;
                }else if(next_char == '*'){
                    state = CMT2;
                    lexeme[lex_spot++] = next_char;
                    next_char = 0;
                }else{
                    state = FINAL;
                    lexeme[lex_spot] =0;
					token_type = DIV_TOK;
                    printf("ACCEPT DIVISION_SIGN %s \n",lexeme);
					return token_type;
					
                }
                break;
            case CMT2:
                if(next_char == '\n')
				{   state = CMT2;
					line_no++;
                    next_char = 0;
				}
				else if (next_char == EOF) {
					printf("ERROR. EOF REACHED WITHIN COMMENT.\n");
					return EOF_TOK;
				}
				else if (next_char == '*') {
					state = CMT3;
					next_char = 0;
				}
				else {
                    state = CMT2;
                    next_char = 0;
                }
                break;
            case CMT3:
				if (next_char == '/') {
					next_char = 0;
					lexeme[lex_spot++] = 0;
					token_type = COMMENT_TOK;
					printf("ACCEPT LONG_COMMENT %s ", lexeme);
					state = FINAL;
					return token_type;
				}
				else if (next_char == '*') {
					state = CMT3;
					next_char = 0;
				}
				
				else {
                    state = CMT2;
                    next_char = 0;
                }
                break;
            case CMT4:
                if(next_char != '\n'){
                    state = CMT4;
                    next_char = 0;
                }else{
                    state = FINAL;
                    token_type = COMMENT_TOK;
                    next_char = 0;
					lexeme[lex_spot++] = 0;
                    //printf("ACCEPT COMMENT %s ",lexeme);
					//return token_type;
					break;
                }
                break;
            case NUM1:
                if ( isdigit(next_char) )
                { state = NUM1;
					if (lex_spot == NUMBER_MAX_LENGTH)
					{
						printf("ERROR: number too long. Truncating. Line %d\n", line_no);
						lexeme[lex_spot++] = 0;
					}
					else if (lex_spot < NUMBER_MAX_LENGTH)
					lexeme[lex_spot++] = next_char;
					next_char = 0;

                }
                else if ( next_char == '.' )
                { state = NUM2;
                  lexeme[lex_spot++] = next_char;
                  next_char = 0;
                }
                else
                { lexeme[lex_spot] = 0; // null for end of string
                  token_type = INTEGER_TOK;
                  printf("ACCEPT INTEGER %s \n",lexeme);  // This is a final state
				  state = FINAL;       // leave next_char alone, for next token
				  return token_type;
				  
                }
                break;

            case NUM2:
                if ( isdigit(next_char) )
                { state = NUM2;
                  lexeme[lex_spot++] = next_char;
                  next_char = 0;
                }
                else
                { lexeme[lex_spot] = 0; // null for end of string
                  token_type = DECIMAL_FRACTION_TOK;
                  printf("ACCEPT DECIMAL_FRACTION %s ",lexeme);  // This is a final state
				  state = FINAL;       // leave next_char alone, for next token
				  return token_type;
                  
                }
                break;
          } // end of switch
        } // end of while state
     }  // end of infinite loop
}// end of main