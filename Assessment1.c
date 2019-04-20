#include <stdio.h>
#include <ctype.h>

/*INCLUDE SHORT FUNCTION FUNCTIONALITY HERE*/


/*
 * to do list
 * 
 * rotation without key, fix sensitivity so it is not 3, but the minimum word size-1
 * substitution cypher
 * comment 
 * 
 */


void copy_array_xz(char *x, char *z, int k);

void encode_caesar(char *x, int k, int key);

void decode_caesarwkey(char *x, int k, int key);

void decode_caesarwokey(char *x, char *z, int k, int *keyr);

void encode_substitution(char *x, int k, char *subrefi);

void create_substitution_key(char *subkeyen, char *subkeyde);

void decode_substitution(char *x, int k, char *subkeyen, char *subkeyde);

void statistical_analysissub(char *x, int k, char *subkeyde);

int statistical_analysisrot(char *x, char *z, int k, int amount);

void create_substitution_keywgkey(char *calcfreq, char *subkeyde);

void decode_substitutionwgkey(char *x, int k, char *calcfreq, char *subkeyde);

void decode_substitutionz(char *z, int k, char *calcfreq, char *subkeyde);

void decode_substitutionwokey(char *x, int k, char *z, char *calcfreq, char *subkeyde, FILE *output);

int main()
{
    int k=0;//k is the size of the array which is made by checking amount of characters in the input file, if not set to 0 then sometimes does not run
    char c;
    int key,select,i;
    int keyr[1];
    char subkeyen[27]=""; //if this is not here then it pus an @ at then end of key
    char subkeyde[26]; //used to make string to decode substitution cypher
    char calcfreq[27]="";
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
    FILE *setup;
    FILE *output;
    
    input=fopen("input.txt", "r");
    setup=fopen("setup.txt", "r");
    output=fopen("output.txt","w");
    
    if(input==NULL){
        perror("Input fopen()");
        return 0;
    }
    if(setup==NULL){
        perror("setup fopen()");
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
    
    fscanf(setup, "%d", &select);
    
    i=0;
    
    if(select<=3){
        fscanf(setup, "%d", &key);
    }
    else    {
        while(i<26){
            fscanf(setup, "%c", &c);
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
            x[i]=' '; //sets i+1 and i+2 to space character, to fill end of array x so there is no non-letters printed
        }
        
    }

    printf("Message from file\n\n%s\n\n",x); //prints the message after being read and converted to upper case
    fprintf(output, "Message from file:\n\n%s\n\n",x);
    
    //                 abcdefghijklmnopqrstuvwxyz   the alphabet
    //char subkeyen[]="ZEBRASCDFGHIJYKLMNOPQTUVWX"; //reference list for substitution encoder/decode
    
    
    

    char z[k];

    //prints selected cypher and key in different ways depending one selection
    switch(select){
        case 1:
            printf("Encoding message using caesar cypher using key: %d\n\n",key);
            encode_caesar(x,k,key); //call encode_caesar()
            printf("%s",x);
            fprintf(output, "Encoding message using caesar cypher using key: %d\n\nEncoded message:\n%s",key,x);
            break;
        case 2:
            printf("Decoding message using caesar cypher using key %d\n\n",key);
            decode_caesarwkey(x,k,key); //call caesar decode_caesarwkey()
            printf("%s",x);
            fprintf(output, "Decoding message using caesar cypher using key: %d\n\nDecoded message:\n%s",key,x);
            break;
        case 3: //need to send to file
            printf("Decoding message using caesar cypher, using brute force\n");
            decode_caesarwokey(x,z,k,keyr); //call decode_caesarwokey() to decode message with out a key being provided
            printf("\nKey found was: %d\n\nMessage decoded using this key\n\n%s",keyr[1],x);
            fprintf(output, "Decoding message using caesar cypher using key found through brute force, key is: %d\n\nDecoded message:\n%s",keyr[1],x);
            //need to add printing to file
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
            //this prints within the function as it is complex and it difficult to print everything in the correct order
            //writing to file is also done within the function to reduce complexity
            printf("Decoding message using substitution cypher using statistical analysis\n\n");
            decode_substitutionwokey(x,k,z,calcfreq,subkeyde,output);
            break;
    }
    fclose(input);
    fclose(setup);
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

void decode_caesarwokey(char *x, char *z, int k,int *keyr){
    FILE *list;
    list=fopen("list.txt", "r");
    
    int n=0,l=0,keyact;
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
    /*printf("List of words that may be in message:\n");
    for(int i=0; i<n; i++){
        for(int j=0; j<20; j++){
            printf("%c",wlist[j][i]);
        }
    }*/
    int p=0;
    
    for(int i=0; i<k; i++){
        if(isspace(x[i])){
            p++;
        }
    }
    //printf("\n\n%d   %d\n\n",p,k);

    char xlist[20][100*p]; //needs to be this big for an unknown reason (should only need to be size p,) ie does not work with small messages when p
    
    for(int i=0; i<k; i++){
        for(int j=0; j<20; j++){
            xlist[j][i]=0;
        }
    }
    
    //printf("%s\n\n",x);
    
    o=0;
    int neg,pos;
    for(int key=0; key<26; key++){
        //printf("%d\n",key);
        copy_array_xz(x,z,k);
        decode_caesarwkey(z,k,key);
        //printf("%s\n\n",z);
        j=0;
        for(int i=0; j<p; i++){
            c=z[i];
            if(isupper(c)){
                xlist[o][j]=c;
                o++;
            }
            else    {
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
                    //if this bit doesn't work then check pos>neg to pos>3
                    else if(isupper(b1)||isupper(a1)){
                        neg+=20;
                    }
                    
                }
                
                if(pos>neg&&neg==0){ //try t replace the '3' with the minimum word size maybe if there is only one word then size-1, else size, do this by counting spaces?
                    /*for(int c=0; c<20; c++){
                        printf("%c",xlist[c][a]);
                    }*/
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
    keyr[1]=keyact; //fix this so it prints correct key (test a <=>26)
    decode_caesarwkey(x,k,keyact);
}

void create_substitution_keywgkey(char *calcfreq, char *subkeyde){
    int l;
    for(int n=0; n<26; n++){
        if(isupper(calcfreq[n])){
            l=calcfreq[n]-65;
            subkeyde[l]=n+65;
        }
    }
}
void decode_substitutionwgkey(char *x, int k, char *calcfreq, char *subkeyde){
    create_substitution_key(calcfreq,subkeyde);
    int code;
    for(int n=0; n<k; n++){
        if(isupper(x[n])){
            code=x[n]-65;
            x[n]=subkeyde[code];
        }
    }
}

void decode_substitutionz(char *z, int k, char *calcfreq, char *subkeyde){
    create_substitution_key(calcfreq,subkeyde);
    int code;
    for(int n=0; n<k; n++){
        if(isupper(z[n])){
            code=z[n]-65;
            z[n]=subkeyde[code];
        }
    }
}

void decode_substitutionwokey(char *x, int k, char *z, char *calcfreq, char *subkeyde, FILE *output){
                  //ABCDEFGHIJKLMNOPQRSTUVWXYZ
    char actfreq[]="ETAOINSRHDLUCMWFGYPBVKJXQZ";

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

    int p=10;
    
    for(int i=0; i<k; i++){
        if(isspace(x[i])||ispunct(x[i])){
            p++;
        }
    }

    char xlist[20][100*p];
    
    for(int i=0; i<k; i++){
        for(int j=0; j<20; j++){
            xlist[j][i]=0;
        }
    }
    int freq[2][26];
        
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
    
    printf("Frequency of letters in message:\n");
    fprintf(output, "Frequency of letters in message:\n");
    
    for(int i=0; i<26; i++){
        printf("%c  %d\n",freq[0][i],freq[1][i]);
    }
    for(int i=0; i<26; i++){
        fprintf(output, "%c  %d\n",freq[0][i],freq[1][i]);
    }
    
    int a;
    
    for(int i=0; i<26; i++){
        a=actfreq[i]-65;
        calcfreq[a]=freq[0][i];
    }
    
    copy_array_xz(x,z,k);
    
    printf("\nKey calculated from statistical analysis: %s\n\n", calcfreq);
    fprintf(output, "\nKey calculated from statistical analysis: %s\n\n", calcfreq);
    
    decode_substitutionz(z,k,calcfreq,subkeyde);

    printf("Message decoded using this key\n\n%s\n\n\n",z);
    fprintf(output, "Message decoded using this key\n%s\n\n\n",z);
    
    j=0;
    for(int i=0; i!=k; i++){
        c=z[i];
        if(isupper(c)||ispunct(c)){
            xlist[o][j]=c;
            o++;
        }    
        else {
            xlist[o][j]=0;
            j++;
            o=0;
        }
    }
    
    int calcheck[27];
    
    for(int i=0; i<27; i++){
        calcheck[i]=0;
    }
    
    
    int pos,neg;
    int as,bs;
    int ac,bc;
    int ai,bi;
    
    /*spell checker*/
    
    for(int b=0; b<p; b++){
        for(int a=0; a<n; a++){
            pos=0;
            neg=0;
            
            for(int c=0; c<20; c++){
                char b1=xlist[c][b];
                char a1=wlist[c][a];
                if(isupper(b1)&&isupper(a1)){
                   if(a1!=b1){
                        neg++;
                    }
                    if(a1==b1){
                        pos++;
                    } 
                }
                else if(isupper(b1)||isupper(a1)){
                    neg+=20;
                }
                
            }

            if(((float)pos/neg)>=1.3){
                for(int c=0; c<20; c++){
                    if(isupper(xlist[c][b])&&isupper(wlist[c][a])){
                        bs=xlist[c][b];
                        as=wlist[c][a];
                        ac=as-65; //what it should be
                        bc=bs-65; //what it is
                        
                        if(calcheck[ac]==0 && calcheck[bc]==0){
                            ai=calcfreq[ac];
                            bi=calcfreq[bc];
                            calcfreq[ac]=bi;
                            calcfreq[bc]=ai;
                            calcheck[ac]=1;
                        }
                    }
                }
                copy_array_xz(x,z,k);
                decode_substitutionz(z,k,calcfreq,subkeyde);
                for(int i=0; i!=k; i++){
                    c=z[i];
                    if(isupper(c)||ispunct(c)){
                        xlist[o][j]=c;
                        o++;
                    }    
                    else {
                        xlist[o][j]=0;
                        j++;
                        o=0;
                    }
                }
                a=n;
            }
        }

    } 
    
    decode_substitution(x,k,calcfreq,subkeyde);
    printf("Key calculated using spell checker: %s\n\n",calcfreq);
    fprintf(output, "Key calculated using spell checker: %s\n\n",calcfreq);
    
    printf("Message decoded using this new key\n\n%s",x);
    fprintf(output, "Message decoded using this new key\n%s",x);


}

