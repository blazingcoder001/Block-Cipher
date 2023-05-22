/// File: encode.c
/// Done by: Jishnuraj Prakasan
/// username: jp4154
/// This file calls the necessary functions to encode the given text and do
/// some basic error checking.

#include "cbc_lib.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/// The main function helps to do some error checking and calls encode 
/// function that is located in cbc_lib.c.
/// @param argc is the number of command line arguments.
/// @param argv is command line arguments stores as strings.
/// @return EXIT_SUCCESS on successful execution of the program and 
/// EXIT_FAILURE on failure of program execution.

int main(int argc, char *argv[])
{
    static  int res;
    if(argc ==1 || argc>2)
    {
      
        fprintf(stderr,"usage: encode to-file-name  # from standard input\n");
    
       
    }
    else
    {
        FILE *inp1,*inp2;
        if((inp1= fopen(argv[1],"r"))==NULL)
        {
              fopen(argv[1],"a");
              res= encode(argv[1]);

        }
        
        else if((inp2= fopen(argv[1],"a"))==NULL) 
        {
            fprintf(stderr,"%s: Permission denied\n",argv[1]);
            fprintf(stderr,"FAILED\n");
        }
        else
        {
            if(inp1!=NULL)            {
                fclose(inp1);
            }
            if(inp2!=NULL)
            {
                fclose(inp2);
            }

            res= encode(argv[1]);
            
            if(res==0)

                return EXIT_SUCCESS;
            else
                return EXIT_FAILURE;
        }
     }


}
