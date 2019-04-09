#include <stdio.h>
#include <ctype.h>

/*INCLUDE SHORT FUNCTION FUNCTIONALITY HERE*/

/*To do list
file i/o
sustitution cypher encode and decode
add dictionary and use it in caesar cypher decryption without key so only the correct output is printed, so there is not 25*/

void mode_auto(int select, char *x,char *z, int k, int key, char *subkeyen, char *subkeyde);

void copy_array_yx(char *x, char *y, int ky);

void copy_array_xz(char *x, char *z, int k);

void scan_code(char *x, int k);

void print_code(char *x, int k);

void encode_caesar(char *x, int k, int key);

void decode_caesarwkey(char *x, int k, int key);

void decode_caesarwokey(char *x, char *z, int k);

void encode_substitution(char *x, int k, char *subrefi);

void create_substitution_key(char *subkeyen, char *subkeyde);

void decode_substitution(char *x, int k, char *subkeyen, char *subkeyde);

int main()
{
    char x[1000];
    int k=sizeof(x)/sizeof(char); //finds amount of elements in char which is used as a constant
    
    
    /*
     * the file layout is:
     * 
     * mode
     * key
     * message to encode/decode
     */
    FILE *input;
    
    char c;
    int key,select;
    input=fopen("input.txt", "r");
    
    if(input==NULL){
        perror("fopen()");
        return 0;
    }
    
    fscanf(input, "%d", &select);
    
    fscanf(input, "%d", &key); //make it chose if 1-3 int key, if not then string key of random alphabet
    
    for(int i=0; i<k; i++){
        fscanf(input, "%c", &c);
        if(feof(input)!=0) break;
        if(islower(c)){
            c-=32;
        }
        x[i]=c;
    }
    fclose(input);
    
    printf("Message from file\n\n"); //prints the message after being converted to capitals
    print_code(x,k);
    

    //intialised array, used to quickly input a large message
    //char y[]="PDA MQFBH ENKUY RKC GQJLO KTAN PDA IZXW SKV";
    //int ky=sizeof(y)/sizeof(char); //amount of elements in intialised array
    
    //initialised selection and key, used instead of an interactive menu, as it is quicker and easier
    //               abcdefghijklmnopqrstuvwxyz   the alphabet
    char subkeyen[]="ZEBRASCDFGHIJYKLMNOPQTUVWX"; //reference list for substitution encoder/decode
    
    
    
    //array used to compare to x in brute force decoding of caesar cypher
    char z[k];
    
    //char subkeysc[26]; //use scanf to put a random alphabet in here
    
    char subkeyde[26];
    
    
    mode_auto(select,x,z,k,key,subkeyen,subkeyde);
    



    
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

/*uses the initialised value for select to select mode and call relevant function, includes relevant prints so it makes
sense in the context of auto select*/
void mode_auto(int select, char *x,char *z, int k, int key, char *subkeyen, char *subkeyde){
    switch(select){
        case 1:
            printf("Encoding message using caesar cypher using key: %d\n\n",key);
            encode_caesar(x,k,key); //call encode_caesar() to encode message
            print_code(x,k); //prints encoded message using print_code()
            break;
        case 2:
            printf("Decoding message using caesar cypher using key %d\n\n",key);
            decode_caesarwkey(x,k,key); //call caesar decode_caesarwkey() function
            print_code(x,k); //prints decoded message using print_code()
            break;
        case 3:
            printf("Decoding message using brute force\n\n");
            decode_caesarwokey(x,z,k); //call decode_caesarwokey() to decode message with out a key being provided
            break;
        case 4:
            printf("Encoding message using substitution cypher\n\n");
            encode_substitution(x,k,subkeyen);
            print_code(x,k);
            break;
        case 5:
            printf("Decoding message using substitution cypher using substitution key");
            decode_substitution(x,k,subkeyen,subkeyde);
            print_code(x,k);
            break;
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

/*copys each element of array y into array x, used in initialised string, as initialised string is constant so it is copied to array x*/
void copy_array_yx(char *x, char *y, int k){
    for(int i=0; i<k; i++){
        x[i]=y[i]; //copies element y[i] into element x[i]
        //is the char is lower case then it is converted to upper case
        if(islower(x[i])){
            x[i]-=32;
            }
        }
    printf("Using initialised message\n\n");
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

/*prompts user to input a message which is saved into array x, which is size k*/
void scan_code(char *x, int k){
    printf("Input message to code/decode: ");
    char in;
    for(int n=0; n<k; n++){ 
        scanf("%c",&in); //inputs message to in variable
        x[n]=in; //each n element of in is assigned to x[n] element of array x
        //is the char is lower case then it is converted to upper case
        if(islower(x[n])){
            x[n]-=32;
        }
        //once the endline symbol is reached, no more elements need to be assigned so the loops ends
        if(x[n]==10){
            n=k;
            break;
        }
    }
    printf("\n");
}

/*prints array to element k or if there is an initialised array then ky is used to make it more efficient*/
void print_code(char *x, int k){ //update this to remove int k and fix comments !!!!!!!!!!!!!!!!!!!!!!
    for(int i=0; x[i]!=0; i++){
        printf("%c",x[i]); //prints array element x[i] until i=k-1, then stop as this is the end of the array
    }
    printf("\n\n");
}

/*encodes array x by a key which is inputed by the user or initialised, finishes at final element k, unless using 
initialised message, then ky is used to prevent overflow*/
void encode_caesar(char *x, int k, int key){
    for(int n=0; n<k; n++){
        //this only encodes capital letters, leaves punctuation and spaces unencoded
        if(isupper(x[n])){
            x[n]=(((x[n]-65)+key)%26)+65; //encode formula, replaces letter x[n] with output from encryption formula
        }
    }

}

/*decodes array x by a key which is inputed by the user or initialised, finishes at final element k, unless using 
initialised message, then ky is used to prevent overflow*/
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
        print_code(z,k);
    }
}