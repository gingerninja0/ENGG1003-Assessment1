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
    input=fopen("input.txt", "r"); //set mode to read, file reads message
    setup=fopen("setup.txt", "r"); //set mode to read, file reads setup
    output=fopen("output.txt","w");//set mode to write, file writes result
    
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
    printf("Message from file input.txt\n\n%s\n\n",x); 
    fprintf(output, "Message from file input.txt\n\n%s\n\n",x);
    
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
            printf("Decoding message using caesar cypher with no key provided\n"); //prints mode to stdout
            decode_caesarwokey(x,z,k,keyr); //call decode_caesarwokey() to decode message without a key being provided
            printf("\nKey found: %d\n\nMessage decoded using this key\n\n%s",keyr[1],x); //print the key that was calculated from an array, and print resulting message, both to stdout
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

/*Decodes the message using the rotation cipher where no key has been provided. A list of words from file list.txt are put into array wlist, and the message is put into array xlist. The message is then decoded using a key and each word from the message is compared to each word from the dictionary. This is repeated for each key, once a word from the message matches a word from the dictionary then that is assumed to be the key used to encode the message, this key is saved and used to decode the entire message.*/
void decode_caesarwokey(char *x, char *z, int k,int *keyr){
    
    
    FILE *list; //declaring a file pointer to list, list contains the dictionary of likely words in the message
    list=fopen("list.txt", "r"); //open list.txt to read words into an array
    
    int n=0,l=0,keyact; //n and l are used to store words, n is the amount of words, calculated by measuring white space and l measures every character. keyact is the key found and used to decode the final message
    char c; //char where characters are stored which are then put into array wlist
    
    //while not at the end of file, count words and characters
    while((c=getc(list))!=EOF){
        //if the character is a space then this must be a new word
        if(isspace(c)){
            n++; //increment word count
        }
        l++; //increment character count
    }
    rewind(list); //put file pointer at start of file
    
    char wlist[20][n]; //create an array the size of the amount of words in the file, each word can be 20 characters long
    
    //NULL array wlist as a precaution
    for(int i=0; i<n; i++){
        for(int j=0; j<20; j++){
            wlist[j][i]=0; //each element of array wlist is NULL-ed
        }
    }
    
    int j=0,o=0; //j is the word position and o is the character posititon, both are initially 0
    
    //this places each word in columns, for each character in the array list
    for(int i=0; i<l; i++){
        fscanf(list, "%c", &c); //read the character the file pointer is at and store it in c
        //if it is lower case make it upper case by subtracting 32 to put it in the range [65,90]
        if(islower(c)){
            c-=32;
        }
        //put the character in to array index wlist[o][j]
        wlist[o][j]=c;
        o++; //increment o so next char goes into the next position of the array
        
        //if there is a space the word must have ended so where there is a space the rest of array wlist[][j] is filled with NULL characters
        if(isspace(c))    {
            //for the rest of the array, make the element NULL
            for(int a=o; a<20; a++){
                wlist[a][j]=0;
            }
            j++; //increment j so the next column is selected
            o=0; //o is reset to 0 so the next word starts at the first position in the array
        }
    }

    int p=0; //p is the amount of words in the inputed message
    
    //while not at the end of message, count words
    for(int i=0; i<k; i++){
        if(isspace(x[i])){
            p++; //increment word count
        }
    }

    char xlist[20][100*p]; //create an array the size of the amount of words in the message, each word can be 20 characters long, must be 100 times largeer for an unknown reason, but it is necessary so don't question it, ignore it and move on
    
    //NULL array xlist as a precaution
    for(int i=0; i<k; i++){
        for(int j=0; j<20; j++){
            xlist[j][i]=0; //each element of array wlist is NULL-ed
        }
    }
    
    
    o=0; //o is the character posititon which is reset to start of column
    int neg,pos; //these are used to score the similarity of one word to another, if a character is identical to another, pos if same, neg if different, after each word they are compared and if pos>neg and neg=0 then the words are identical
    
    /*Spell Checker*/
   
   //cycles through each key until one creats a word that exactally matches another word
    for(int key=0; key<26; key++){
        copy_array_xz(x,z,k); //call copy_array_xz(), array z[] now contains the message which will be used to test keys
        decode_caesarwkey(z,k,key); //the message is decoded using the key from the for loop, this increments each loop
        j=0; //j is set to 0 so the first word of the message is put into the first position of array xlist[][]
        //this cycles through each word of the message and puts them into array xlist
        for(int i=0; j<p; i++){
            c=z[i]; //put element z[i] into c
            //if the character is upper case the letter is added to array xlist[][], elst set the character to a space and cycle to the next word
            if(isupper(c)){
                xlist[o][j]=c; //puts the upper case letter into array element xlist[o][j]
                o++; //increment o so the next letter goes into the next position
            }
            else    {
                xlist[o][j]=32; //if its not an upper case letter, set to a space
                j++; //increment j to select the next word
                o=0;  //o is reset to 0 so the next word starts at the first position in the array
            }
        }
        
        //increment through each word of the message for each key
        for(int b=0; b<p; b++){
            //increment through each word of the dictionary for each word of the message
            for(int a=0; a<n; a++){
                //reset scores
                pos=0;
                neg=0;
                
                //increment through each letter of the selected word of the message and list
                for(int c=0; c<20; c++){
                    char b1=xlist[c][b]; //b1 is the letter of the word from the message
                    char a1=wlist[c][a]; //a1 is the letter of the word from the dictionary
                    //if the letters are both upper case, they are compared
                    if(isupper(b1)&&isupper(a1)){
                        //if they are different the neg value is incremented
                        if(a1!=b1){
                            neg++;
                            }
                        //if they are the same the pos value is incremented    
                        if(a1==b1){
                            pos++;
                        } 
                    }
                    //if one of the characters are not upper case then the words must be different lengths and therefore different words so neg is incremented
                    else if(isupper(b1)||isupper(a1)){
                        neg++;
                    }
                }
                //after the full words are compared then pos and neg values are compared, if pos>neg and neg=0 then the words must be the same
                if(pos>neg && neg==0){
                    keyact=key; //the guessed key must then be the actualt key, this is stored as keyact as the key that is found
                    key=26; //the main loop incrementing through keys is then quit
                    b=p; //the message loop is quit
                    a=n; //the dictionary loop is quit
                    //these force an exit from the spell checker//
                }
            }
        }
    }
    keyr[1]=keyact; //keyact is stored in array keyr[1] to be used to print in main
    decode_caesarwkey(x,k,keyact); //the original message is then decoded using the calculated key, the resulting message is then printed in main
}


void decode_substitutionwokey(char *x, int k, char *z, char *calcfreq, char *subkeyde, FILE *output){
                  //ABCDEFGHIJKLMNOPQRSTUVWXYZ //the alphabet, used to match letter in debugging
    char actfreq[]="ETAOINSRHDLUCMWFGYPBVKJXQZ"; //the frequency of letters in the english language

    FILE *list; //declaring a file pointer to list, list contains the dictionary of likely words in the message
    list=fopen("list.txt", "r"); //open list.txt to read words into an array
    
    int n=0,l=0; //n and l are used to store words, n is the amount of words, calculated by measuring white space and l measures every character. keyact is the key found and used to decode the final message
    char c; //char where characters are stored which are then put into array wlist
    
    //while not at the end of file, count words and characters
    while((c=getc(list))!=EOF){
        //if the character is a space then this must be a new word
        if(isspace(c)){
            n++; //increment word count
        }
        l++; //increment character count
    }
    rewind(list); //put file pointer at start of file
    
    char wlist[20][n]; //create an array the size of the amount of words in the file, each word can be 20 characters long

    //NULL array wlist as a precaution
    for(int i=0; i<n; i++){
        for(int j=0; j<20; j++){
            wlist[j][i]=0; //each element of array wlist is NULL-ed
        }
    }
    
    int j=0,o=0; //j is the word position and o is the character posititon, both are initially 0
    
    //this places each word in columns, for each character in the array list
    for(int i=0; i<l; i++){
        fscanf(list, "%c", &c); //read the character the file pointer is at and store it in c
        //if it is lower case make it upper case by subtracting 32 to put it in the range [65,90]
        if(islower(c)){
            c-=32;
        }
        //put the character in to array index wlist[o][j]
        wlist[o][j]=c;
        o++; //increment o so next char goes into the next position of the array
        
        //if there is a space the word must have ended so where there is a space the rest of array wlist[][j] is filled with NULL characters
        if(isspace(c))    {
            //for the rest of the array, make the element NULL
            for(int a=o; a<20; a++){
                wlist[a][j]=0;
            }
            j++; //increment j so the next column is selected
            o=0; //o is reset to 0 so the next word starts at the first position in the array
        }
    }

    int p=0; //p is the amount of words in the inputed message
    
    //while not at the end of message, count words
    for(int i=0; i<k; i++){
        if(isspace(x[i])){
            p++; //increment word count
        }
    }

    char xlist[20][100*p]; //create an array the size of the amount of words in the message, each word can be 20 characters long, must be 100 times largeer for an unknown reason, but it is necessary so don't question it, ignore it and move on
    
    //NULL array xlist as a precaution
    for(int i=0; i<k; i++){
        for(int j=0; j<20; j++){
            xlist[j][i]=0; //each element of array wlist is NULL-ed
        }
    }
    
    int freq[2][26]; //array which stores letters and letter freqency freq[0][] is the letter and freq[1][] is the amount of times that letter occurs
        
    int t; //t stores the index of the upper case letter which is used to compare with the frequency of letters in english
    int n1; //n1 is the sorting index, 0 is highest, 25 is lowest
    int m=26; //the amount of letters in the key, must be 26 as there is 26 letters in the alphabet
    int x1,x2; //x1 and x2 store the letter in the index freq[1][n1] and freq[1][n1+1] respectivly
    int y1,y2; //y1 and y2 store the freqency of the letter in the message in x1 and x2 respectivly
    
    //Bubble Sorter//
    
    //zeros each letters frequency
    for(int i=0; i<26; i++){
        freq[0][i]=i+65; //i+65 puts it in the range of [65,90], so each capital letter is put into array freq[0][i]
        freq[1][i]=0; //the corresponding frequency is set to 0 
    }
    
    //counts the frequency of each letter in the message
    for(int i=0; i<k; i++){
        //if the letter is upper case the corresponding frequenct is incremented
        if(isupper(x[i])){
            t=x[i]-65; //in upper case letter is shifted from [65,90] to [0,25] and is used as the index used to increment the frequency
            freq[1][t]++; //frequency of the letter is incremented
        }
    }
    
    //Sorting Algorithm//
    
    //cycles through list of frequencies until they are in order from highest to lowest
    for(n1=0; n1<m-1; n1++){
        x1=freq[1][n1]; //the frequency of the letter y1
        y1=freq[0][n1]; //the letter in freq[0][n1]
        x2=freq[1][n1+1]; //the frequency of the letter in y2
        y2=freq[0][n1+1]; //the letter in freq[0][n1+1]
        //if the letter freq[0][n1+1] has a higher freqency then freq[0][n1], they are swapped. If they are equal then they remain in their positions, this can sometimes cause problems!, to fix swap the offending letters in the actfreq[] string
        if(x2>x1){
            //the letter and frequency swap positions in array freq[][]
            freq[1][n1]=x2; //frequency of y2 is put in position of y1
            freq[0][n1]=y2; //letter x2 is put in position of x1
            freq[1][n1+1]=x1; //frequency of y1 is put in position of y2
            freq[0][n1+1]=y1; //letter x1 is put in position of x2
            n1=-1; //n1 is set to -1 so when the for loop increments it is set to 0 (the initial value)
        }
    }
    //describes what will be printed after these
    printf("Frequency of letters in English followed by frequency of letters in message:\n\n");
    fprintf(output, "Frequency of letters in English followed by frequency of letters in message:\n\n");
    
    for(int i=0; i<26; i++){
        //prints the english letter frequency highest to lowest, then the message letter frequency highest to lowest, and its relevant frequency score to stdout and file output.txt
        printf("%c   %c   %d\n",actfreq[i],freq[0][i],freq[1][i]);
        fprintf(output, "%c   %c  %d\n",actfreq[i],freq[0][i],freq[1][i]);
    }
    
    int a; //a stores the index of the letter from actfreq[] which is used to create calcfreq[], which is used to decode the message
    
    //this creats a key which hopefully contains part of the key used to encode the message, the rest is made using a spell check below
    //puts each letter from actfreq[] in the range [0,25], this is the actual frequency of letters in english, the letter used to encode the respective letter is matched and stored in calcfreq[a]
    for(int i=0; i<26; i++){
        a=actfreq[i]-65; //find the index of the letter in range [0,25]
        calcfreq[a]=freq[0][i]; //a is then used as the index of calcfreq[a] to store the letter found using statistical analysis, from freq[0][i]
    }
    
    copy_array_xz(x,z,k); //call copy_array_xz() to save store the original message so all the testing is done on a copy
    
    //print the key calculated from statistical analysis to stdout and output.txt
    printf("\nKey calculated from statistical analysis: %s\n\n", calcfreq);
    fprintf(output, "\n\nKey calculated from statistical analysis: %s\n\n", calcfreq);
    
    decode_substitutionz(z,k,calcfreq,subkeyde); //call decode_substitutionz() to decode the copied message using the calculated key
    
    //print the message decoded using the key calculated from statistical analysis
    printf("Message decoded using this key\n\n%s\n\n\n",z);
    fprintf(output, "Message decoded using this key\n\n%s\n\n\n",z);

    j=0; //j is set to 0 so the first word of the message is put into the first position of array xlist[][]
    //this cycles through each word of the message and puts them into array xlist
    for(int i=0; i!=k; i++){
        c=z[i]; //put element z[i] into c
        //if the character is upper case the letter is added to array xlist[][], elst set the character to a space and cycle to the next word
        if(isupper(c)||ispunct(c)){
            xlist[o][j]=c; //puts the upper case letter into array element xlist[o][j]
            o++; //increment o so the next letter goes into the next position
        }    
        else {
            xlist[o][j]=0; //if its not an upper case letter, set to NULL
            j++; //increment j to select the next word
            o=0; //o is reset to 0 so the next word starts at the first position in the array
        }
    }
    
    int calcheck[27]; //initialise an array of the same size as calcfreq, this is used to remember if a letter has been swapped or not
    //sets each value to zero as none of the letters in the key have been swapped
    for(int i=0; i<27; i++){
        calcheck[i]=0;
    }
    
    int pos,neg; //these are used to score the similarity of one word to another, if a character is identical to another, pos if same, neg if different, after each word they are compared and if pos>neg and neg=0 then the words are identical
    int as,bs;
    int ac,bc;
    int ai,bi;
    
    /*Spell Checker*/
    
    //increment through each word of the message
    for(int b=0; b<p; b++){
        //increment through each word of the dictionary for each word of the message
        for(int a=0; a<n; a++){
            //reset pos and neg score before word comparison
            pos=0;
            neg=0;
            
            //increment through each letter of the selected word of the message and list
            for(int c=0; c<20; c++){
                char b1=xlist[c][b]; //b1 is the letter of the word from the message
                char a1=wlist[c][a]; //a1 is the letter of the word from the dictionary
                //if the letters are both upper case, they are compared
                if(isupper(b1)&&isupper(a1)){
                    //if they are different the neg value is incremented
                    if(a1!=b1){
                        neg++;
                    }
                    //if they are the same the pos value is incremented
                    if(a1==b1){
                        pos++;
                    } 
                }
                //if one of the characters are not upper case then the words must be different lengths and therefore different words so neg is made a large number so pos/neg is never greater than or equal to 1.3
                else if(isupper(b1)||isupper(a1)){
                    neg+=20;
                }
            }
            //after the full words are compared then pos and neg values are compared, if pos/neg is >=1.3 the words are assumed to be the same as most of the letters in each word match
                for(int c=0; c<20; c++){
                    
                    if(isupper(xlist[c][b]) && isupper(wlist[c][a])){
                        bs=xlist[c][b]; //b1 is the letter of the word from the message
                        as=wlist[c][a]; //a1 is the letter of the word from the dictionary
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