/*

File Name: main.c
Course: Compiler Theory
Project Title: Variables
Author: Hasana Chaudry
Date: April 18th, 2017
Description: A component file of the Main Driver

*/
#include "atto-C.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

FILE *sourcefile;
FILE *asmfile;

int main(int argc, char **argv){
	  char asmfilename[100];
	 
	  strncpy(asmfilename,argv[1],95);
    sourcefile = fopen(argv[1],"r");
     if ( sourcefile == NULL){
	         perror(argv[1]);
	         exit(1);     
	  }
    int length;
    length = strlen(asmfilename);
    strcpy(asmfilename+length-2,".asm");
    asmfile = fopen(asmfilename,"w");
    if(asmfile==NULL){
	     perror(asmfilename);
	     exit(9);
  	}
    preamble(argv[1]);
	program();
    postamble();
       

}


