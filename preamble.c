/*

File Name: preamble.c
Course: Compiler Theory
Project Title: Variables
Author: Hasana Chaudry
Date: April 18th, 2017
Description: A supporting file for the Main Driver

*/
#include "atto-C.h"

void preamble(char *sourcefilename){
     fprintf(asmfile,  "      TITLE %s\n" ,sourcefilename);
     fprintf(asmfile,  "      .686P\n");
     fprintf(asmfile,  "      .model flat\n\n");
     fprintf(asmfile,  "INCLUDELIB MSVCRTD \n");
     fprintf(asmfile,  "INCLUDELIB OLDNAMES\n");
}


void postamble(){
	fprintf(asmfile,"END\n");
	fclose(asmfile);
}
