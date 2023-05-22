/// File: decode.c
/// Done by Jishnuraj Prakasan
/// Uername: jp4154
/// This file helps to call the functions to decode the message that are 
/// located in cbc_lib.c file

#include "cbc_lib.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/// This function helps to do some error checking and calls decode function /// located in cbc_lib.c file.
/// @param argc is the number of command line arguments.
/// @param argv is the command line arguments stored as strings.
/// @return EXIT_SUCCESS on successful execution or EXIT_FAILURE on failure
/// to execute execute the program.
int main(int argc,char *argv[])
{
	
    if(argc==1 || argc>2)
    {
       fprintf(stderr,"usage: decode file-name  # to standard input\n\n");

    }
    else
    {
        FILE *inp1,*inp2;
        if((inp1=fopen(argv[1],"r"))==NULL)
        {
            fprintf(stderr,"%s: No such file or directory\n",argv[1]);
            fprintf(stderr,"FAILED\n");

        }
        else if((inp2=fopen(argv[1],"a"))==NULL)
        {
            fprintf(stderr,"%s: Permission denied\n",argv[1]);
            fprintf(stderr,"FAILED\n");
        }
        else
        {
            if(inp1!=NULL)
            {
                fclose(inp1);
            }
            if(inp2!=NULL)
            {
                fclose(inp2);
            }

            int res=decode(argv[1]);
            if(res==0)
            {  
                
                return EXIT_SUCCESS;
            }
            else
            {
                
                return EXIT_FAILURE;
            }
               
        }

    }
}
