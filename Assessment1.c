#include <stdio.h>
#include <ctype.h>

/*INCLUDE HIGH LEVEL FUNCTIONALITY OF PROGRAM HERE INCLUDING STUDENT ID AND PROJECT NAME*/

void copy_array_xz(char *x, char *z, int k);

void encode_caesar(char *x, int k, int key);

void decode_caesarwkey(char *x, int k, int key);

void decode_caesarwokey(char *x, char *z, int k, int *keyr);

void encode_substitution(char *x, int k, char *subrefi);

void create_substitution_key(char *subkeyen, char *subkeyde);

void decode_substitution(char *x, int k, char *subkeyen, char *subkeyde);

void create_substitution_keywgkey(char *calcfreq, char *subkeyde);

void decode_substitutionwgkey(char *x, int k, char *calcfreq, char *subkeyde);

void decode_substitutionz(char *z, int k, char *calcfreq, char *subkeyde);

void decode_substitutionwokey(char *x, int k, char *z, char *calcfreq, char *subkeyde, FILE *output);

/*main() contains reading and writing to file (most of the time) and also handles mode selection*/
int main()
{
    int k=10; //k is the size of the array that the message is put into, k is made by checking amount of characters in the input file
    char c; //a char used to store char's read from files
    int key,select,i=0; //key is the key used in the rotation cipher, select is the mode chosen, i is a counter used throughout main and initialised to 0
    int keyr[1]; //used to send the key back to main in decoding a rotation cipher with an unknown key
    char subkeyen[27]=" "; //array used to store 26 letter key which is inputed in file, is set to size 27 and includes a space character so the key is printed correctly
    char subkeyde[26]; //array used to store the 26 letter decode key which is created using the funtcion create_substitution_key and create_substitution_keywgkey
    char calcfreq[27]=" "; //array used to store the frequency of letters from substitution decription, is set to size 27 and includes a space character so the key is printed correctly
    
    //declaring file pointers, input is where the initial message is, setup is mode and key selection, output is where the message is sent after executing mode
    FILE *input;
    FILE *setup;
    FILE *output;
    
    //open each file
    input=fopen("input.txt", "r"); //set mode to read
    setup=fopen("setup.txt", "r"); //set mode to read
    output=fopen("output.txt","w");//set mode to write
    
    //if any of the files don't exist then print and error and stop the program by returning 0
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
    
    //this finds the size of the message, by checking if at end of file and while it is not adding 1 to k
    while((c=getc(input))!=EOF){
        k++; //increment size by 1
    }
    rewind(input); //resets to start of file after reading size, ready for inputing message into array
    
    //creates array of size k, which was calculated previously, this is done to maximise efficiency and reduce ram usage
    char x[k]; //array where the message is stored
    
    //used to find where to set file pointer, delete before upload/////////////////////////////////////////////////////////////////////////////////////////
    /*int isel=0;
    int position=0;
    
    while((c=getc(setup))!=EOF){
        position++;
    }
    rewind(input);
    printf("\n\n%d\n\n",position);*///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    fseek(setup, 387, SEEK_SET ); //sets file pointer to character 387 to skip over menu and jump to where the mode is selected
        
    fscanf(setup, "%d", &select); //read this character to select mode
    
    fseek(setup, 405, SEEK_SET ); //sets file pointer to character 405 to skip over menu and mode select and jump to where key begins
    
    //if the selected mode is 1 or 2 then the key must be an integer, so it is read as such, else the key must be a string of 26 letters
    //if mode 3 or 6 is selected then the key inputed is ignored to improve ease of use
    if(select<3){
        fscanf(setup, "%d", &key); //puts integer into key
    }
    else if(select!=3 && select!=6){
        //reads all 26 integers into array subkeyen, starting index at 0 using i initialised previously
        while(i<26){
            fscanf(setup, "%c", &c); //read char and put into c
            //if the key is lower case, then it is made upper case by subtracting 32 to shift it from [97,122] to [65,90]
            if(islower(c)){
                c-=32;
            }
            subkeyen[i]=c; //this char is then put into index i of array subkeyen
            i++; //i is then incremented to put next char into the next element of array subkeyen
        }
    }
    
    //puts message into element i of array x
    for(i=0; i<k; i++){
        fscanf(input, "%c", &c); //read char and put into c
        //if not at the end of the file then read character into element i of array x
        if(feof(input)==0){
            //if the message includes a newline character, this is included into message so it is a perfect copy of what was inputed
            if(c==10){
                i--;
            }
            //if the key is lower case, then it is made upper case by subtracting 32 to shift it from [97,122] to [65,90]
            if(islower(c)){
                c-=32;
            }
            x[i]=c; //puts character into element i of array x
  
        }
        //when at end of file the extra elements of the array are filled with NULL to stop unwanted letters being printed
        else    {
            //while not at end of array put NULL into element i of array x, then increment i
            while(i<k){
                x[i]=0;
                i++;
            }
        }
    }
    //prints the message to stdout and file output.txt, after being read and converted to upper case
    printf("Message from file\n\n%s\n\n",x); 
    fprintf(output, "Message from file:\n\n%s\n\n",x);
    
    char z[k]; //creates an array identical to array x. This is used to copy array x into array z to save the message from manipulation

    //using mode selected and key, the respective messages are printed to indicate what mode was selected and what key was used and also the resulting message
    
    switch(select){
        case 1: //mode is encode using rotation cipher
            printf("Encoding message using caesar cypher using key: %d\n\n",key); //prints mode and key to stdout
            encode_caesar(x,k,key); //call encode_caesar() to encode message using rotation cipher with a selected key
            printf("%s",x); //print resulting message to stdout
            fprintf(output, "Encoding message using caesar cypher using key: %d\n\nEncoded message:\n\n%s",key,x); //print mode, key and resulting message to file output.txt
            break;
        case 2: //mode is decode using rotation cipher with a known key
            printf("Decoding message using caesar cypher using key %d\n\n",key); //prints mode and key to stdout
            decode_caesarwkey(x,k,key); //call decode_caesarwkey() to decode message using rotation cipher with a selected key
            printf("%s",x); //print resulting message to stdout
            fprintf(output, "Decoding message using caesar cypher using key: %d\n\nDecoded message:\n\n%s",key,x); //print mode, key and resulting message to file output.txt
            break;
        case 3: //mode is decode using rotation cipher with an unknown key
            printf("Decoding message using caesar cypher, using spell checker\n"); //prints mode to stdout
            decode_caesarwokey(x,z,k,keyr); //call decode_caesarwokey() to decode message without a key being provided
            printf("\nKey found was: %d\n\nMessage decoded using this key\n\n%s",keyr[1],x); //print the key that was calculated from an array, and print resulting message, both to stdout
            fprintf(output, "Decoding message using caesar cypher using key found through spell checking, key is: %d\n\nDecoded message:\n\n%s",keyr[1],x); //print mode, calculated key and resulting message to file output.txt
            break;
        case 4: //mode is encode using substitution cipher
            printf("Encoding message using substitution cypher with key: %s\n\n",subkeyen); //prints mode and key to stdout
            encode_substitution(x,k,subkeyen); //call encode_substitution() to encode message using substitution cipher with a selected key
            printf("%s",x); //print resulting message to stdout
            fprintf(output, "Encoding message using substitution cypher using key: %s\n\nEncoded message:\n\n%s",subkeyen,x); //print mode, key and resulting message to file output.txt
            break;
        case 5: //mode is decode using substitution cipher with a known key
            printf("Decoding message using substitution cypher using substitution key: %s\n\n",subkeyen); //prints mode and key to stdout
            decode_substitution(x,k,subkeyen,subkeyde); //call decode_substitution() to decode message using substitution cipher with a selected key
            printf("%s",x); //print resulting message to stdout
            fprintf(output, "Decoding message using substitution cypher using key: %s\n\nEncoded message:\n\n%s",subkeyen,x); //print mode, key and resulting message to file output.txt
            break;    
        case 6: //mode is decode using substitution cipher with an unknown key
            //this prints within the function as it is complex and is difficult to print everything in the correct order
            //writing to file is also done within the function to reduce complexity
            //prints mode to stdout and file output.txt
            printf("Decoding message using substitution cypher using statistical analysis\n\n");
            fprintf(output, "Decoding message using substitution cypher using statistical analysis\n\n");
            decode_substitutionwokey(x,k,z,calcfreq,subkeyde,output); //call decode_substitutionwokey() to decode message using substitution cipher without a key being provided
            break;
    }
    //close all the files
    fclose(input);
    fclose(setup);
    fclose(output);
    
    return 0;
}

/*Decodes the message using the subtitution cipher. This is done by creating a key which is a reverse of what was inputed. Using this key the message is decoded by checking if element x[i] is upper case, if it is the result is an integer in the range [0,25] this is then used as the index of subkeyde[] which reads the element in that index and puts that into element x[i] as the new decoded letter*/
void decode_substitution(char *x, int k, char *subkeyen, char *subkeyde){
    create_substitution_key(subkeyen,subkeyde); //call create_substitution_key() to create the key used to decode message
    int code; //integer used as the index of letter in key subkeyde[]
    //this for() loop increments through message decoding upper case letters
    for(int i=0; i<k; i++){
        //if the element x[i] is upper case it is decoded
        if(isupper(x[i])){
            code=x[i]-65; //finds the element in x[i] which is subtracted by 65 to put it in the range [0,25], this is stored in code. This is then used as the index for the respective letter in subkeyde[]
            x[i]=subkeyde[code]; //finds element in subkeyde[code] which is then put into x[i], which is the decoded letter
        }
    }
}

/*Encodes the message using the substitution cipher. This is done by checking if element x[i] is upper case, if it is the result is an integer in the range [0,25], this is then used as the index of subkeyen[] which reads the element in that index and puts that into element x[i] as the new encoded letter*/
void encode_substitution(char *x, int k, char *subkeyen){
    int code; //integer used as the index of letter in key subkeyen[]
    //this for() loop increments through message encoding upper case letters
    for(int i=0; i<k; i++){
        //if the element x[i] is upper case it is encoded
        if(isupper(x[i])){
            code=x[i]-65; //finds the element in x[i] which is subtracted by 65 to put it in the range [0,25], this is stored in code. This is then used as the index for the respective letter in subkeyen[]
            x[i]=subkeyen[code]; //finds element in subkeyen[code] which is then put into x[i], which is the encoded letter
        }    
    }
}

/*Copys each element of array x into array z. This is done by finding element x[i] and putting it into element z[i]*/
void copy_array_xz(char *x, char *z, int k){
    //this for() loop increments through message copying each element x[i] into element z[i]
    for(int i=0; i<k; i++){
        z[i]=x[i]; //copies element x[i] into element z[i]
    }
}

/*Creates the key used for decoding the substitution cipher. This is done by finding the element in subkeyen[i], 65 is then subtracted and this is stored in l. l becomes the index of subkeyde[] where i+65 is stored (i+65, so i is shifted to the range [65,90])*/
void create_substitution_key(char *subkeyen, char *subkeyde){
    int l; //index of subkeyde[]
    //this for() loop increments through [0,25] to put each letter i+65 into the respective index subkeyde[]
    for(int i=0; i<26; i++){
        l=subkeyen[i]-65; //finds letter in index i of subkeyen[], 65 is then subtracted to put this into the range [0,25], this is then stored in l and is used as the index to store the letter i+65
        subkeyde[l]=i+65; //puts i+65 (so it is upper case in the range [65,90]) into the index subkeyde[l], which creats a key which is used to decode the message
    }
}

/*Encodes the message using the rotation cipher. This is done by checking if element x[i] is upper case, if it is the key is added to the element*/
void encode_caesar(char *x, int k, int key){
    //this for() loop increments through message encoding upper case letters
    for(int i=0; i<k; i++){
        //if the element x[i] is upper case it is encoded
        if(isupper(x[i])){
            x[i]=(((x[i]-65)+key)%26)+65; //encode formula, x[i]-65 to put into range [0,25], then the key is added, then the modulus is taken, then 65 is added to put the result into the range [65,90] so it is an upper case letter
        }
    }
}

/*Decodes the message using the rotation cipher. This is done by checking if element x[i] is upper case, if it is the key is subtracted from the element*/
void decode_caesarwkey(char *x, int k, int key){
    //this for() loop increments through message decoding upper case letters
    for(int i=0; i<k; i++){
        //if the element x[i] is upper case it is decoded
        if(isupper(x[i])){
            x[i]=((26+(x[i]-65)-key)%26)+65; //decode formula, x[i]-65 to put into range [0,25], then 26 is added so if (x[i]-65)-key is <0 the modulus operator will work (adding 26 does nothing in other cases as the modulus of 26 is 0 which does not effect the answer) then the key is added, then the modulus is taken, then 65 is added to put the result into the range [65,90] so it is an upper case letter
        }
    }
}

//The next 2 functions are similar to some previous functions but are only called when decoding substitution cipher with no key provided

/*Decodes the message using the subtitution cipher once a key has been found. This is done by creating a key which is a reverse of what was calculated. Using this key the message is decoded by checking if element x[i] is upper case, if it is the result is an integer in the range [0,25] this is then used as the index of subkeyde[] which reads the element in that index and puts that into element x[i] as the new decoded letter*/
void decode_substitutionwgkey(char *x, int k, char *calcfreq, char *subkeyde){
    create_substitution_key(calcfreq,subkeyde); //call create_substitution_key() to create the key used to decode message
    int code; //integer used as the index of letter in key subkeyde[]
    ///this for() loop increments through message decoding upper case letters
    for(int i=0; i<k; i++){
        //if the element x[i] is upper case it is decoded
        if(isupper(x[i])){
            code=x[i]-65; //finds the element in x[i] which is subtracted by 65 to put it in the range [0,25], this is stored in code. This is then used as the index for the respective letter in subkeyde[]
            x[i]=subkeyde[code]; //finds element in subkeyde[code] which is then put into x[i], which is the decoded letter
        }
    }
}

/*Decodes the message using the subtitution cipher, used to update z[] once a letter has been swapped. This is done by creating a key which is a reverse of what was calculated. Using this key the message is decoded by checking if element z[i] is upper case, if it is the result is an integer in the range [0,25] this is then used as the index of subkeyde[] which reads the element in that index and puts that into element z[i] as the new decoded letter*/
void decode_substitutionz(char *z, int k, char *calcfreq, char *subkeyde){
    create_substitution_key(calcfreq,subkeyde); //call create_substitution_key() to create the key used to decode message
    int code; //integer used as the index of letter in key subkeyde[]
    for(int i=0; i<k; i++){
        //if the element x[i] is upper case it is decoded
        if(isupper(z[i])){
            code=z[i]-65; //finds the element in z[i] which is subtracted by 65 to put it in the range [0,25], this is stored in code. This is then used as the index for the respective letter in subkeyde[]
            z[i]=subkeyde[code]; //finds element in subkeyde[code] which is then put into z[i], which is the decoded letter
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

    int p=0;
    
    for(int i=0; i<k; i++){
        if(isspace(x[i])){
            p++;
        }
    }

    char xlist[20][100*p]; //needs to be this big for an unknown reason (should only need to be size p,) ie does not work with small messages when p
    
    for(int i=0; i<k; i++){
        for(int j=0; j<20; j++){
            xlist[j][i]=0;
        }
    }
    
    o=0;
    int neg,pos;
    for(int key=0; key<26; key++){
        copy_array_xz(x,z,k);
        decode_caesarwkey(z,k,key);
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
                
                if(pos>neg && neg==0){
                    keyact=key;
                    key=26;
                    b=p;
                    a=n;
                    pos=0;
                    neg=0;
                }
            }
        }
    }
    keyr[1]=keyact; //fix this so it prints correct key (test a <=>26)
    decode_caesarwkey(x,k,keyact);
}

void decode_substitutionwokey(char *x, int k, char *z, char *calcfreq, char *subkeyde, FILE *output){
                  //ABCDEFGHIJKLMNOPQRSTUVWXYZ
    char actfreq[]="ETAOINSRHDLUCMWFGYPBVKXJQZ";

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
    
    printf("Frequency of letters in: English followed by message:\n");
    fprintf(output, "Frequency of letters in: English followed by message:\n");
    
    for(int i=0; i<26; i++){
        printf("%c   %c  %d\n",actfreq[i],freq[0][i],freq[1][i]);
        fprintf(output, "%c   %c  %d\n",actfreq[i],freq[0][i],freq[1][i]);
    }
    
    int a;
    
    for(int i=0; i<26; i++){
        a=actfreq[i]-65;
        calcfreq[a]=freq[0][i];
    }
    
    copy_array_xz(x,z,k);
    
    printf("\nKey calculated from statistical analysis: %s\n\n", calcfreq);
    fprintf(output, "\n\nKey calculated from statistical analysis: %s\n\n", calcfreq);
    
    decode_substitutionz(z,k,calcfreq,subkeyde);

    printf("Message decoded using this key\n\n%s\n\n\n",z);
    fprintf(output, "Message decoded using this key\n\n%s\n\n\n",z);
    
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

            if(((float)pos/neg)>=2){
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
    fprintf(output, "Message decoded using this new key\n\n%s",x);
}