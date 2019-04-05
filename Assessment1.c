#include <stdio.h>
#include <ctype.h>

/*INCLUDE SHORT FUNCTION FUNCTIONALITY HERE*/

/*To do list
file i/o
sustitution cypher encode and decode
add dictionary and use it in caesar cypher decryption without key so only the correct output is printed, so there is not 25*/

void mode_auto(int select, char *x,char *z, int k, int ky, int key);

void mode_select(int select, char *x, char *z, int k, int ky, int key);

void copy_array_yx(char *x, char *y, int ky);

void copy_array_xz(char *x, char *z, int k);

void scan_code(char *x, int k);

void print_code(char *x, int k, int ky);

void encode_caesar(char *x, int k, int ky, int key);

void decode_caesarwkey(char *x, int k, int ky, int key);

void decode_caesarwokey(char *x, char *z, int k, int ky);

void encode_substitution(char *x, int k, char *subrefi);

int main()
{   
    //intialised array, used to quickly input a large message
    char y[]="The quick brown fox jumps over a dog";
    int ky=sizeof(y)/sizeof(char); //amount of elements in intialised array
    
    //initialised selection and key, used instead of an interactive menu, as it is quicker and easier
    //int select=2; //list selections here!
    //int key=9; //key used for decoding and encoding quickly
    
    
    
    
    
    char x[1000];
    
    int k=sizeof(x)/sizeof(char); //finds amount of elements in char which is used as a constant
    int code;
    //array used to compare to x in brute force decoding of caesar cypher
    //char z[k];
    
    //             abcdefghijklmnopqrstuvwxyz   the alphabet
    
    char subrefi[]="ZYXWVUTSRQPONMLKJIHGFEDCBA"; //reference list for substitution encoder
    
    char subkeyi[]="ZYXWVUTSRQPONMLKJIHGFEDCBA"; //where key to decode substitution cypher goes, can also use scanf to input this
    
    char subkeys[26];
    
    /*if an array is initialised, then it skips using scan_code, the copies string to array x using copy_array_yx
    so it can be changed in the program*/
    if(ky!=1){
        copy_array_yx(x,y,ky);
        print_code(x,k,ky);
    } 
    /*else read user message using scan_code()*/
    else    {
        scan_code(x,k);
    }
    
    encode_substitution(x,k,subrefi);
    
    print_code(x,k,ky);
    
    for(int n=0; n<k; n++){
        if(isupper(x[n])){
            code=x[n]-65;
            x[n]=subkeyi[code];
        }    
    }
    print_code(x,k,ky);
    
    /*if there is not mode selected through initialisation then read user inputs using mode_select()*/
    //if(select==0){
        //mode_select(select,x,z,k,ky,key);
    //}
    /*else there must be an initialised selection, use mode_auto() to show relevant mode selected*/
    //else {
        //mode_auto(select,x,z,k,ky,key);
    //}
    
    return 0;
}


void deencode_substitution(char *x, int k, char *subrefi){
    int mode, code;
    printf("Use initialised substitution or type in key\n");
    printf("1=initialised substitution\n");
    printf("2=substitution from file\n\n");
    printf("Select mode: ");
    scanf("%d", &mode);
    printf("\n");
    
    for(int n=0; n<k; n++){
        if(isupper(x[n])){
            code=x[n]-65;
        switch(mode){
            case 1:
                x[n]=subrefi[code];
                break;
            case 2: //this is where it will encode from file
                x[n]=subrefi[code];
                break;
            }
        }    
    }
}

void encode_substitution(char *x, int k, char *subrefi){
    int mode, code;
    printf("Use initialised substitution or type in key\n");
    printf("1=initialised substitution\n");
    printf("2=substitution from file\n\n");
    printf("Select mode: ");
    scanf("%d", &mode);
    printf("\n");
    
    for(int n=0; n<k; n++){
        if(isupper(x[n])){
            code=x[n]-65;
        switch(mode){
            case 1:
                x[n]=subrefi[code];
                break;
            case 2: //this is where it will encode from file
                x[n]=subrefi[code];
                break;
            }
        }    
    }
}

/*uses the initialised value for select to select mode and call relevant function, includes relevant prints so it makes
sense in the context of auto select*/
void mode_auto(int select, char *x,char *z, int k, int ky, int key){
    switch(select){
        case 1:
            printf("Encoding message using caesar cypher using key: %d\n\n",key);
            encode_caesar(x,k,ky,key); //call encode_caesar() to encode message
            print_code(x,k,ky); //prints encoded message using print_code()
            break;
        case 2:
            printf("Decoding message using caesar cypher using key %d\n\n",key);
            decode_caesarwkey(x,k,ky,key); //call caesar decode_caesarwkey() function
            print_code(x,k,ky); //prints decoded message using print_code()
            break;
        case 3:
            printf("Decoding message using brute force\n\n");
            decode_caesarwokey(x,z,k,ky); //call decode_caesarwokey() to decode message with out a key being provided
            break;
    }
}

/*prompts user to select mode and calls relevant function*/
void mode_select(int select, char *x, char *z, int k, int ky, int key){
    printf("Mode Selection Menu\n");
    printf("1: encode using caesar cypher\n");
    printf("2: decode caesar cypher with known key\n");
    printf("3: decode caesar cypher using brute force\n\n");
    printf("Select mode: ");
    scanf("%d",&select);
    printf("\n");
    switch(select){
        case 1:
            encode_caesar(x,k,ky,key); //call encode_caesar() to encode message
            print_code(x,k,ky);  //prints encoded message using print_code()
            break;
        case 2:
            decode_caesarwkey(x,k,ky,key); //call caesar decode_caesarwkey() function
            print_code(x,k,ky); //prints decoded message using print_code()
            break;
        case 3:
            decode_caesarwokey(x,z,k,ky); //call decode_caesarwokey() to decode message with out a key being provided
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
void copy_array_yx(char *x, char *y, int ky){
    for(int i=0; i<ky; i++){
        x[i]=y[i]; //copies element y[i] into element x[i]
        //is the char is lower case then it is converted to upper case
        if(islower(x[i])){
            x[i]-=32;
            }
        }
    printf("Using initialised message\n\n");
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
void print_code(char *x, int k, int ky){
    if(ky!=1){
        k=ky;
    }
    for(int i=0; x[i]!=0; i++){
        printf("%c",x[i]); //prints array element x[i] until i=k-1, then stop as this is the end of the array
    }
    printf("\n\n");
}

/*encodes array x by a key which is inputed by the user or initialised, finishes at final element k, unless using 
initialised message, then ky is used to prevent overflow*/
void encode_caesar(char *x, int k, int ky, int key){
    if(ky!=1){
        k=ky;
    }
    //input key if one is not initialised at start
    if(key==0){
        printf("Select key to use to encode: ");
        scanf("%d",&key);
    }
    for(int n=0; n<k; n++){
        //this only encodes capital letters, leaves punctuation and spaces unendoded
        if(isupper(x[n])){
            x[n]=(((x[n]-65)+key)%26)+65; //encode formula, replaces letter x[n] with output from encryption formula
        }
    }
}

/*decodes array x by a key which is inputed by the user or initialised, finishes at final element k, unless using 
initialised message, then ky is used to prevent overflow*/
void decode_caesarwkey(char *x, int k, int ky, int key){
    if(ky!=1){
        k=ky;
    }
    //input key if one is not initialised at start
    if(key==0){
        printf("Select key to use to decode: ");
        scanf("%d",&key);
    }
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
void decode_caesarwokey(char *x, char *z, int k, int ky){
    printf("One of the keys will give the correct output, scroll through 26 outputs until one matches and note the key displayed\n\n");
    if(ky!=1){
        k=ky;
    }
    for(int key=1; key<26; key++){
        copy_array_xz(x,z,k); //creates a copy of array x which is shifted by key, repeats 25 times
        for(int n=0; n<k; n++){
            if(isupper(z[n])){
                z[n]=((26+(z[n]-65)-key)%26)+65;
            }
        }
        printf("Key %d gives the message:\n",key);
        print_code(z,k,ky);
    }
}