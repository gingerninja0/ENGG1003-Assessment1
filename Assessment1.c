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
            printf("Decoding message using brute force\n\n");
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
    }
    fclose(input);
    fclose(output);
    
    
    printf("\n\n");
    
    int freq[26];
   
    for(int i=0; i<26; i++){
        freq[i]=0;

    }
    
    int t;
    for(int i=0; i<k; i++){
        t=x[i]-65;
        freq[t]++;
    }
    
    for(int i=0; i<26; i++){
        printf("%c   %d\n",i+65,freq[i]);
    }
    int m=26;
    int n,n1; 
    int xn,x1,x2;
    
    
    for(n=0; n<m; n++){
        xn=freq[n];
        printf("%d,",xn);
    }
    printf("\n\n");
    
    
    
    for(n1=0; n1<m-1; n1++){
        x1=freq[n1];
        x2=freq[n1+1];
        if(x2>x1){
            freq[n1]=x2;
            freq[n1+1]=x1;
            n1=-1;
        }
    }   

    for(n=0; n<m; n++){ 
        xn=freq[n];
        printf("%d,",xn);
    }

    /*
    int freq[3][26];
    
    char x[]="RCR VYE BGBX HBNX FHB FXNQBRV YM RNXFH IZNQEBCJ FHB PCJB? C FHYEQHF KYF. CF'J KYF N JFYXV FHB DBRC PYEZR FBZZ VYE. CF'J N JCFH ZBQBKR. RNXFH IZNQEBCJ PNJ N RNXA ZYXR YM FHB JCFH, JY IYPBXMEZ NKR JY PCJB HB LYEZR EJB FHB MYXLB FY CKMZEBKLB FHB OCRCLHZYXCNKJ FY LXBNFB ZCMB… HB HNR JELH N AKYPZBRQB YM FHB RNXA JCRB FHNF HB LYEZR BGBK ABBI FHB YKBJ HB LNXBR NWYEF MXYO RVCKQ. FHB RNXA JCRB YM FHB MYXLB CJ N INFHPNV FY ONKV NWCZCFCBJ JYOB LYKJCRBX FY WB EKKNFEXNZ. HB WBLNOB JY IYPBXMEZ… FHB YKZV FHCKQ HB PNJ NMXNCR YM PNJ ZYJCKQ HCJ IYPBX, PHCLH BGBKFENZZV, YM LYEXJB, HB RCR. EKMYXFEKNFBZV, HB FNEQHF HCJ NIIXBKFCLB BGBXVFHCKQ HB AKBP, FHBK HCJ NIIXBKFCLB ACZZBR HCO CK HCJ JZBBI. CXYKCL. HB LYEZR JNGB YFHBXJ MXYO RBNFH, WEF KYF HCOJBZM.";
    
    char actfreq[]="ETAOINSRHDLUCMFYWGPBVKXQJZ";
    
    char calcfreq[27]="";
    
    int k=sizeof(x);
    
    int t;
    
    int m=26;
    int n,n1; 
    int xn,x1,x2;
    char y1,y2;
    
    for(int i=0; i<26; i++){
        freq[0][i]=actfreq[i];
    }
    
    for(int i=0; i<26; i++){
        freq[1][i]=i+65;
        freq[2][i]=0;
    }
    
    for(int i=0; i<k; i++){
        if(isupper(x[i])){
            t=x[i]-65;
            freq[1][t]=x[i];
            freq[2][t]++; 
        }
        
    }
    
    for(n1=0; n1<m-1; n1++){
        x1=freq[2][n1];
        y1=freq[1][n1];
        x2=freq[2][n1+1];
        y2=freq[1][n1+1];
        if(x2>x1){
            freq[2][n1]=x2;
            freq[1][n1]=y2;
            freq[2][n1+1]=x1;
            freq[1][n1+1]=y1;
            n1=-1;
        }
    }
    
    
    
    
    for(int i=0; i<26; i++){
        printf("%c   %c  %d\n",freq[0][i],freq[1][i],freq[2][i]);
    }
    
    int a,b,c;
    
    for(int i=0; i<26; i++){
        calcfreq[i]=freq[1][i];
    }
    
    printf("%s", calcfreq);
    */
    
    
    
    
    
    
    
    
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