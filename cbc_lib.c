 // File: cbc_lib.c
/// Done by: Jishnuraj Prakasan
/// username: jp4154
/// This file has definitions of all the necessary functions to 
/// perform encryption and decryption of messaes.

#include<stdio.h>
#include<stdlib.h>
#include"cbc_lib.h"
#include<string.h>

/// This macro defines the number of bytes of block64

#define BYTES_PER_BLOCK sizeof(long long)

///This macro defines the total  number of bits in in byte.

#define BITS_PER_BYTE 8L

///This macro defines the total number of bits per block of block64(one variable).

#define BITS_PER_BLOCK (BYTES_PER_BLOCK*BITS_PER_BYTE)

/// This macro defines the input size of the input which can be increased 
/// according to the size of the input.

#define INPUT_SIZE (24* BYTES_PER_BLOCK)

/// This variable holds the key for the encryption and decryption.

static block64 key= 0x1234DeadBeefCafe;

///This variable holds the value of the initialization vector.

static const block64 INITIALIZATION_VECTOR= 0x0L;

/// This function helps to do the bit barrel roll to the left.
/// @param block holds a single block for which the operation is to be 
/// performed.
/// @param count holds the number of times the bit barrel roll is to be 
/// performed.
/// @return the block after the operation.

block64 roll_left(block64 block, size_t count)
{
    block64 shift;
    size_t k;
    k=64-count;
    shift=(block<<count)|(block>>k);
    return shift;
}

/// This function helps to do the XOR operation between the block given and
/// the key.
/// @param block is a single block passed to do the operation.
/// @param key is the key to do xor operation.
/// @return the block after the operation.

block64 block_cipher_encrypt(block64 block, block64 key)
{
    block64 result;
    result=block^key;
    return result;
}

/// This function helps to convert the text passed to it to block64 type 
/// and calls roll_left and block_cypher_encrypt functions.
/// @param text is the input text to be encoded.
/// @param pIV is the initialization vector passed.
/// @param key is the key that is used for encryption and decryption.
/// @return a block64 type array of the input message. 

block64 *cbc_encrypt(char *text, block64 *pIV, block64 key)
{
    int len,i,j,cpy;int r=8;
    float  mul8;int k=0;int lower;int ind=0;int flg1=0;
    char *plain;block64 x;
    static block64 *ini;
    i=1;len=0;
    len=strlen(text);
    plain=calloc(len+1,sizeof(char));
    strcpy(plain,text);
    mul8=len/8.0;
    if(mul8-(int)mul8 ==0)
    {
        ini=calloc((int)mul8,sizeof(block64));
    }
    else
    {

        ini=calloc((int)mul8+1,sizeof(block64));
    }
   cpy=len;
   while(cpy>0)
   {
       r=8;
       flg1=0;
       lower=k;
       if(cpy>=8)
       {
           i=7+k;
       }

       else
       {
           i=cpy-1+k;
       }

       for(;i>=lower;i--)
       {
           if(flg1==1)
           {
               ini[ind]=ini[ind]+(((block64)plain[i])<<r);
               r=r+8;
           }
           else
           {
               ini[ind]=(block64)plain[i];
               flg1=1;
           }

       }
       if(cpy-8>0)
       {
           ind++;
           k=k+8;

       }
       cpy=cpy-8;

   }
   len=ind+1;
 
   for(i=0;i<len;i++)
   {
       x=ini[i] ^ *pIV;
       for(j=0;j<4;j++)
       {
           x=roll_left(x,10);          
           x=block_cipher_encrypt(x,key);

       }
       *pIV=x;
       ini[i]=x;
       

   }
   free(plain);
   return ini;
}

/// Used to encode the given message into the file located in destpath.

int encode(const char *destpath)
{
    int len;int mul8len;int oldpos;int newpos;
    len=0;
    char *plain,*copy;block64 *final;
    plain=calloc(1,sizeof(char));
    copy=plain;
    int charsum=1;
    int d;
    float  mul8;int flg2;
    FILE *f;FILE *out;
    f=fopen(destpath,"r");
    oldpos=ftell(f);
    fseek(f,0,SEEK_END);
    newpos=ftell(f);
    if(oldpos!=newpos)
    {
        fseek(f,0,SEEK_SET);
        d=fread(plain,sizeof(char),1,f);
        charsum++;
        while(d>0)
        {
            plain=copy;
            plain=(char *)realloc(plain,charsum);
            copy=plain;
            plain=plain+charsum-1;
            d=fread(plain,sizeof(char),1,f);
            charsum++;
        }
        fprintf(stderr,"ok\n");

    }
    else
    {
        d=fread(plain,sizeof(char),1,stdin);
        charsum++;
        while(d>0 )
        {
            plain=copy;
            plain=(char *)realloc(plain,charsum);
            copy=plain;
            plain=plain+charsum-1;
            d=fread(plain,sizeof(char),1,stdin);
            charsum++;
        }
        if(plain!=NULL)
        {
            fprintf(stderr,"ok\n");
        }
        else
        {
            fprintf(stderr,"%s: No such file or directory\n",destpath);
            fprintf(stderr,"FAILED\n");

        }
     }
     charsum=charsum-2;
     plain=copy;
     plain[charsum]='\0';
     fclose(f);

   len=strlen(plain);
   mul8=len/8.0;
   if(mul8-(int)mul8 ==0)
   {
       mul8len=(int)mul8;
   }
   else
   {
       mul8len=(int)mul8+1;
   }
   block64 *iv=calloc(1,sizeof(block64));
   *iv=INITIALIZATION_VECTOR;
   out=fopen(destpath,"wb");
   final= cbc_encrypt(plain,iv, key);
   flg2=fwrite(final, mul8len*sizeof(block64),1,out);
   fclose(out);
   plain=copy;
   free(plain);
   free(final);
   free(iv);
   if(flg2)
   {
       return EXIT_SUCCESS;
   }
   else
   {
       return EXIT_FAILURE;
   }

}

/// This function helps to convert the given block64 data to string.
/// @param txt is of block64 data type and has the decrypted data.
/// @param data is the character array that will hold the decrypted and
/// original string.
/// @return nothing.

void block64_to_string(block64 txt,char *data)
{
    int len;int r=56;int i,start;
    unsigned char *cpy=calloc(1,sizeof(unsigned char));
    len=strlen(data);

    start=len;
    for(i=0;i<8;i++)
    {
        if(i!=0)
        {
            txt=txt<<8;
        }
        *cpy=(unsigned char)(txt>>r);
        if(*cpy!=0)
        {
            data[start]=*cpy;
            start++;
        }
        
    }
    data[start]='\0';
    free(cpy);
}

/// This function helps to do XOR operation between the given block
/// and the key.
/// @param block has the data in block64 datatype.
/// @param key is the key used for encryption and decryption.
/// @return a block64 data type variable that holds the result of the 
/// operation.

block64 block_cipher_decrypt(block64 block, block64 key)
{
    block=block^key;
    return block;
}

/// This function helps to do bit barrel right operation for the given count.
/// @param block has a single block of data in block64 type.
/// @param count is the number of times the bit  barrel roll has to be performed.
/// @return the block after the operation.

block64 roll_right(block64 block, size_t count)
{
    block64 shift;size_t k;
    k=64-count;
    shift=(block>>count)|(block<<k);
    return shift;
}

/// This function calls the block_cipher_decrypt and roll_right fucntions
/// to do the decryption operation.
/// Also it calls the block64_to_string fucntion to convrt the decrypted 
/// block64 type of data to string.
/// @param ciphertext is the block64 type of data to decode.
/// @param count is the number of blocks of block64 data passed.
/// @param pIV is the initialization vector that can hold the intial iv and /// intermediate values.
/// @param key hold the key for this encryption and decryption.
/// @return a character pointer containing the decrypted string.

char *cbc_decrypt(block64 *ciphertext, size_t count, block64 *pIV, block64 key)
{
    size_t i,j;block64 x;
    for(i=0;i<count;i++)
    {
        x=ciphertext[i];
        for(j=0;j<4;j++)
        {
            x= block_cipher_decrypt(x,key);
            x=roll_right(x,10);
        }
        x=x^*pIV;
        *pIV=ciphertext[i];
        ciphertext[i]=x;

    }
    static char *plaintext;
    plaintext = calloc(count*8,sizeof(char));
    plaintext[0]='\0';
    for(i=0;i<count;i++)
    {
        block64_to_string(ciphertext[i],plaintext);
    
    }
    return plaintext;
}

/// Helps to decrypt the data in sourcepath and print to the standard output.

int decode(const char * sourcepath)
{
    FILE *inp;int d;size_t len=0;
    static char *plain=NULL;
    inp=fopen(sourcepath,"rb");
    block64 *read, *conv;
    read=calloc(1,sizeof(block64));
    d=fread(read,sizeof(block64),1,inp);
    if(d>0)
    {
        conv=calloc(1,sizeof(block64));
        conv[len]=*read;
        len++;
    }
    while(d>0)
    {
        d=fread(read,sizeof(block64),1,inp);
        if(d>0)
        {
            conv=realloc(conv,(len+1)*sizeof(block64));
            conv[len]=*read;
            len++;
        }
    }
    block64 *iv= calloc(1,sizeof(block64));
    *iv=INITIALIZATION_VECTOR;   
    plain=cbc_decrypt(conv,len,iv,key);
    int res=printf("%s",plain);
    free(iv);
    free(conv);
    free(read);
    fclose(inp);
    if(res>=0)
    {
        fprintf(stderr,"ok\n");
        return EXIT_SUCCESS;
    }
    else
    {
    
        return EXIT_FAILURE;

    }
}
