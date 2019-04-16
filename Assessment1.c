#include <stdio.h>
#include <ctype.h>

/*INCLUDE SHORT FUNCTION FUNCTIONALITY HERE*/


/*
 * to do list
 * 
 * statistical analysis
 *      find most common letter and assign it E, then second most common letter A, etc
 * 
 * 
 */


void copy_array_xz(char *x, char *z, int k);

void encode_caesar(char *x, int k, int key);

void decode_caesarwkey(char *x, int k, int key);

void decode_caesarwokey(char *x, char *z, int k);

void encode_substitution(char *x, int k, char *subrefi);

void create_substitution_key(char *subkeyen, char *subkeyde);

void decode_substitution(char *x, int k, char *subkeyen, char *subkeyde);

void statistical_analysissub(char *x, int k, char *subkeyde);

int statistical_analysisrot(char *x, char *z, int k, int amount);

int main()
{
    int k=0;//k is the size of the array which is made by checking amount of characters in the input file, if not set to 0 then sometimes does not run
    char c;
    int key,select,i;
    char subkeyen[27]=""; //if this is not here then it pus an @ at then end of key
    char subkeyde[26]; //used to make string to decode substitution cypher
    
    /*
     * the file Input layout is:
     * 
     * mode
     * key
     * message to encode/decode (last character must be a space)
     * 
     */
    
    //File io from 51 to 108
    
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
    
    //while not at EOF add to k+1 which is size of file
    while((c=getc(input))!=EOF){
        k++;
    }
    rewind(input); //resets to start of file after reading size
    
    char x[k]; //makes array slightly larger than required so it is more efficient
    
    fscanf(input, "%d", &select);
    
    if(select<=3){
        fscanf(input, "%d", &key);
    }
    else    {
        while(i<26){
            fscanf(input, "%c", &c);
            if(isupper(c)){
                subkeyen[i]=c;
                i++;
            }
        }
    }
    
    //puts message into array x
    for(i=0; i<k; i++){
        fscanf(input, "%c", &c);
        //if not at EOF then read character into x
        if(feof(input)==0){
            //if the character is a lower case letter, make it upper case and then add to array x 
            if(islower(c)){
                c-=32;
            }
            x[i]=c;
  
        }
        else    {
            x[i]=0; //fills rest of array with 0 when at end of file
        }
        
    }

    printf("Message from file\n\n%s\n\n",x); //prints the message after being read and converted to upper case
    
    //                 abcdefghijklmnopqrstuvwxyz   the alphabet
    //char subkeyen[]="ZEBRASCDFGHIJYKLMNOPQTUVWX"; //reference list for substitution encoder/decode
    
    
    
    //array used to compare to x in brute force decoding of caesar cypher probably not needed anymore
    char z[k];

    //prints selected cypher and key in different ways depending one selection
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
            printf("Decoding message using caesar cypher, using brute force\n\n");
            decode_caesarwokey(x,z,k); //call decode_caesarwokey() to decode message with out a key being provided
            break;
        case 4:
            printf("Encoding message using substitution cypher with key: %s\n\n",subkeyen);
            encode_substitution(x,k,subkeyen);
            printf("%s",x);
            fprintf(output, "Encoding message using substitution cypher using key: %s\n\nEncoded message:\n%s",subkeyen,x);
            break;
        case 5:
            printf("Decoding message using substitution cypher using substitution key: %s\n\n",subkeyen);
            decode_substitution(x,k,subkeyen,subkeyde);
            printf("%s",x);
            fprintf(output, "Decoding message using substitution cypher using key: %s\n\nEncoded message:\n%s",subkeyen,x);
            break;    
        case 6:
            printf("Decoding message using substitution cypher using statistical analysis\n\n");
            statistical_analysissub(x,k,subkeyen);
            decode_substitution(x,k,subkeyen,subkeyde);
            printf("\nThe key made is: %s\n\n",subkeyen);
            printf("%s",x);
            fprintf(output, "Decoding message using substitution cypher using statistical analysis used the calculated key: %s\n\nDecoded message:\n%s",subkeyen,x);
            break;
    }
    fclose(input);
    fclose(output);
    
    
    
    
    
    
    return 0;
}

void statistical_analysissub(char *x, int k, char *subkeyen){
    int freq[2][26];
    char actfreq[]="ETAOINSRHDLUCMFYWGPBVKXQJZ";
    
    
    
    int t;
    
    int m=26;
    int n1; 
    int x1,x2;
    int y1,y2;
    
    
    for(int i=0; i<26; i++){
        freq[0][i]=i+65;
        freq[1][i]=0;
    }
    
    for(int i=0; i<k; i++){
        if(isupper(x[i])){
            t=x[i]-65;
            freq[0][t]=x[i];
            freq[1][t]++; 
        }
        
    }
    
    for(n1=0; n1<m-1; n1++){
        x1=freq[1][n1];
        y1=freq[0][n1];
        x2=freq[1][n1+1];
        y2=freq[0][n1+1];
        if(x2>x1){
            freq[1][n1]=x2;
            freq[0][n1]=y2;
            freq[1][n1+1]=x1;
            freq[0][n1+1]=y1;
            n1=-1;
        }
    }
    
    
    
    
    for(int i=0; i<26; i++){
        printf("%c  %d\n",freq[0][i],freq[1][i]);
    }
    
    int a;
    
    for(int i=0; i<26; i++){
        a=actfreq[i]-65;
        subkeyen[a]=freq[0][i];
    }
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

void decode_caesarwokey(char *x, char *z, int k){
    FILE *list;
    list=fopen("list.txt", "r");
    
    int n=0,l=0;
    char c;
    
    while((c=getc(list))!=EOF){
        if(isspace(c)){
            n++;
        }
        l++;
    }
    //printf("%d   %d\n\n",n,l);
    rewind(list);
    
    char wlist[20][n];

    for(int i=0; i<n; i++){
        for(int j=0; j<20; j++){
            wlist[j][i]=0;
        }
    }
    
    int j=0,o=0;;
    
    for(int i=0; i<l; i++){
        fscanf(list, "%c", &c);
        
        if(islower(c)){
            c-=32;
        }
        wlist[o][j]=c;
        o++;
            
        if(isspace(c))    {
            for(int a=o; a<20; a++){
                wlist[a][j]=0;
            }
            j++;
            o=0;
        }
    }
    printf("List of words that may be in message:\n");
    for(int i=0; i<n; i++){
        for(int j=0; j<20; j++){
            printf("%c",wlist[j][i]);
        }
    }
    int p=0;
    
    for(int i=0; i<k; i++){
        if(isspace(x[i])){
            p++;
        }
    }
    //printf("\n\n%d\n\n",p);

    char xlist[20][p];
    
    for(int i=0; i<p; i++){
        for(int j=0; j<20; j++){
            xlist[j][i]=0;
        }
    }
    
    //printf("%s\n\n",x);
    
    o=0;
    int keyact,neg,pos;
    
    for(int key=0; key<26; key++){
        //printf("%d\n",key);
        copy_array_xz(x,z,k);
        decode_caesarwkey(z,k,key);
        //printf("%s\n\n",z);
        j=0;
        for(int i=0; j!=p; i++){
            c=z[i];
            if(isupper(c)){
                xlist[o][j]=c;
                o++;
            }    
            else {
                xlist[o][j]=32;
                j++;
                o=0;
            }
        }
        /*for(int i=0; i<p; i++){
            for(int j=0; j<20; j++){
                printf("%c",xlist[j][i]);
            }
            printf(" ");
        }*/
       
        for(int b=0; b<p; b++){ 
            for(int a=0; a<n; a++){
                pos=0;
                neg=0;
                for(int c=0; c<20; c++){
                    char b1=xlist[c][b];
                    char a1=wlist[c][a];
                    if(isupper(b1)){
                        if(a1!=b1){
                            neg++;
                        }
                        if(a1==b1){
                            pos++;
                        }
                        //printf("%c  ",wlist[c][0]);
                        //printf("pos=%d   neg=%d\n",pos,neg);
                    }
                    
                }
                
                
                if(pos>2&&neg==0){ //this is length of word in file -1, ie mon which is 2 and everybody which is 8
                    for(int c=0; c<20; c++){
                        //printf("%c",xlist[c][a]);
                    }
                    //printf("key=%d   word is:%d    pos=%d    neg=%d\n",key,a,pos,neg);
                    keyact=key;
                    key=26;
                    b=p;
                    a=n;
                    pos=0;
                    neg=0;
                    //printf("key is: %d\n",keyact);
                }
            }
        }
        for(int i=0; i<n; i++){
            for(int j=0; j<20; j++){
                xlist[j][i]=0;
            }
        }
    }
    
    decode_caesarwkey(x,k,keyact);
    printf("\n\nKey found was: %d\n\nMessage decoded using this key\n\n%s",keyact,x);
}