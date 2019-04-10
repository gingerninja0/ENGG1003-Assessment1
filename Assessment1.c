#include <stdio.h>
#include <ctype.h>

/*INCLUDE SHORT FUNCTION FUNCTIONALITY HERE*/


/*
 * to do list
 * finish file i/o
 * file key selection int or string
 * statistical analysis
 */


void copy_array_xz(char *x, char *z, int k);

void encode_caesar(char *x, int k, int key);

void decode_caesarwkey(char *x, int k, int key);

void decode_caesarwokey(char *x, char *z, int k);

void encode_substitution(char *x, int k, char *subrefi);

void create_substitution_key(char *subkeyen, char *subkeyde);

void decode_substitution(char *x, int k, char *subkeyen, char *subkeyde);

int main()
{
    int k=-2;//k is the size of the array which is made by checking amount of characters in the input file, is set to -2 so the 2 characters at end are ignored
    char c;
    int key,select;
    
    /*
     * the file Input layout is:
     * 
     * mode
     * key
     * message to encode/decode
     * 
     */
    
    
    FILE *input;
    FILE *output;
    
    input=fopen("input.txt", "r");
    output=fopen("output.txt","w");
    
    if(input==NULL){
        perror("Input fopen()");
        return 0;
    }
    if(output==NULL){
        perror("Output fopen()");
        return 0;
    }
    while((c=getc(input))!=EOF){
        k++;
    }
    rewind(input);
    
    //creates an array of size k
    char x[k];
    
    fscanf(input, "%d", &select);
    
    /*if(select<=3){
        fscanf(input, "%d", &key);
    }*/
    
    fscanf(input, "%d", &key); //make it chose if 1-3 int key, if not then string key of random alphabet
    
    //puts message into array x
    for(int i=0; i<k; i++){
        fscanf(input, "%c", &c);
        if(feof(input)!=0) i=k;
        if(islower(c)){
            c-=32;
        }
        x[i]=c;
    }
    fclose(input);
    
    printf("Message from file\n\n%s\n\n",x); //prints the message after being converted to capitals
    
    //               abcdefghijklmnopqrstuvwxyz   the alphabet
    char subkeyen[]="ZEBRASCDFGHIJYKLMNOPQTUVWX"; //reference list for substitution encoder/decode
    
    
    
    //array used to compare to x in brute force decoding of caesar cypher
    char z[k];
    // these two are archaic and will probably be removed later
    char subkeyde[26];

    switch(select){
        case 1:
            printf("Encoding message using caesar cypher using key: %d\n\n",key);
            encode_caesar(x,k,key); //call encode_caesar()
            printf("%s",x);
            fprintf(output, "Encoding message using caesar cypher using key: %d\n\nEncoded message:\n\n%s",key,x);
            break;
        case 2:
            printf("Decoding message using caesar cypher using key %d\n\n",key);
            decode_caesarwkey(x,k,key); //call caesar decode_caesarwkey()
            printf("%s",x);
            fprintf(output, "Decoding message using caesar cypher using key: %d\n\nDecoded message:\n\n%s",key,x);
            break;
        case 3:
            printf("Decoding message using brute force\n\n");
            decode_caesarwokey(x,z,k); //call decode_caesarwokey() to decode message with out a key being provided
            break;
        case 4:
            printf("Encoding message using substitution cypher with key: %s\n\n",subkeyen);
            encode_substitution(x,k,subkeyen);
            printf("%s",x);
            fprintf(output, "Encoding message using substitution cypher using key: %s\n\nEncoded message:\n\n%s",subkeyen,x);
            break;
        case 5:
            printf("Decoding message using substitution cypher using substitution key: %s\n\n",subkeyen);
            decode_substitution(x,k,subkeyen,subkeyde);
            printf("%s",x);
            fprintf(output, "Decoding message using substitution cypher using key: %s\n\nEncoded message:\n\n%s",subkeyen,x);
            break;
    }
    
    return 0;
}

void decode_substitution(char *x, int k, char *subkeyen, char *subkeyde){
    create_substitution_key(subkeyen,subkeyde);
    int code;
    for(int n=0; n<k; n++){
        if(isupper(x[n])){
            code=x[n]-65;
            x[n]=subkeyde[code];
        }
    }
}

void encode_substitution(char *x, int k, char *subkeyen){
    int code;
    for(int n=0; n<k; n++){
        if(isupper(x[n])){
            code=x[n]-65;
            x[n]=subkeyen[code];
        }    
    }
}

/*copys each element of array x into array z, used in brute force decoding of caesar cypher*/
void copy_array_xz(char *x, char *z, int k){
    for(int i=0; i<k; i++){
        z[i]=x[i]; //copies element x[i] into element z[i]
        //is the char is lower case then it is converted to upper case
        if(islower(x[i])){
            x[i]-=32;
            }
        }
}

void create_substitution_key(char *subkeyen, char *subkeyde){
    int l;
    for(int n=0; n<26; n++){
        if(isupper(subkeyen[n])){
            l=subkeyen[n]-65;
            subkeyde[l]=n+65;
        }
    }
}

/*encodes array x by a key which is read from file input.txt, finishes at final element k*/
void encode_caesar(char *x, int k, int key){
    for(int n=0; n<k; n++){
        //this only encodes capital letters, leaves punctuation and spaces unencoded
        if(isupper(x[n])){
            x[n]=(((x[n]-65)+key)%26)+65; //encode formula, replaces letter x[n] with output from encryption formula
        }
    }

}

/*decodes array x by a key which is read from file input.txt, finishes at final element k*/
void decode_caesarwkey(char *x, int k, int key){
    for(int n=0; n<k; n++){
        //this only decodes capital letters, leaves punctuation and spaces undecoded
        if(isupper(x[n])){
            x[n]=((26+(x[n]-65)-key)%26)+65; //decode formula, replaces letter x[n] with output from decryption formula
        }
    }
}

/*lists 25 messages each shifted by a different key, finishes at final element k, unless using 
initialised message, then ky is used to prevent overflow. After each new key, the key is printed and the message is printed using
print_code(). copies array x using copy_array_xz() into array z each loop so it is not over written*/
void decode_caesarwokey(char *x, char *z, int k){
    printf("One of the keys will give the correct output, scroll through 26 outputs until one matches and note the key displayed\n\n");
    for(int key=1; key<26; key++){
        copy_array_xz(x,z,k); //creates a copy of array x which is shifted by key, repeats 25 times
        for(int n=0; n<k; n++){
            if(isupper(z[n])){
                z[n]=((26+(z[n]-65)-key)%26)+65;
            }
        }
        printf("Key %d gives the message:\n",key);
        printf("%s",z);
    }
}