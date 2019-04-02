#include <stdio.h>

void mode_auto(int select, char *x,char *z, int k, int ky, int key);

void mode_select(int select, char *x, char *z, int k, int ky, int key);

void copy_array_yx(char *x, char *y, int ky);

void copy_array_xz(char *x, char *z, int k);

void scan_code(char *x, int k);

void print_code(char *x, int k, int ky);

void encode_caesar(char *x, int k, int ky, int key);

void decode_caesarwkey(char *x, int k, int ky, int key);

void decode_caesarwokey(char *x, char *z, int k, int ky);

int main()
{
    char in; //what scanf inputs into
    int k,ky,key=0,select=0; //k is array size constant, key is what the array is shifted by, select is used to select mode
    char x[1000];
    
    k=sizeof(x)/sizeof(char); //finds amount of elements in char which is used as a constant
    
    //array used to compare to x
    char z[k];
    
    //intialised array
    char y[]="GUR DHVPX OEBJA SBK WHZCF BIRE N YNML QBT";
    //initialised selection and key
    select=2; //list selections!
    key=13;
    
    
    
    ky=sizeof(y)/sizeof(char); //elements in intialised array
    
    
    
    if(ky!=1){
        copy_array_yx(x,y,ky);
        print_code(x,k,ky);
    }    
    else    {
        scan_code(x,k);
    }
    
    if(select==0){
        mode_select(select,x,z,k,ky,key);
    }
    else {
        mode_auto(select,x,z,k,ky,key);
    }
    
    
    
    
    
    
    
    
    
    
    

    return 0;
}

void mode_auto(int select, char *x,char *z, int k, int ky, int key){
    switch(select){
        case 1:
            printf("Encoding message using caesar cypher using key: %d\n\n",key);
            encode_caesar(x,k,ky,key); //call caesar encoder function
            print_code(x,k,ky);
            break;
        case 2:
            printf("Decoding message using caesar cypher using key %d\n\n",key);
            decode_caesarwkey(x,k,ky,key); //call caesar decoder function
            print_code(x,k,ky);
            break;
        case 3:
            printf("Decoding message using brute force\n\n");
            decode_caesarwokey(x,z,k,ky);
            break;
    }
}

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
            encode_caesar(x,k,ky,key); //call caesar encoder function
            print_code(x,k,ky);
            break;
        case 2:
            decode_caesarwkey(x,k,ky,key); //call caesar decoder function
            print_code(x,k,ky);
            break;
        case 3:
            decode_caesarwokey(x,z,k,ky); //call brute force decoder function
            break;
                
    }
}

void copy_array_xz(char *x, char *z, int k){
    for(int i=0; i<k; i++){
        z[i]=x[i];
        if(islower(x[i])){
            x[i]-=32;
            }
        }
}

void copy_array_yx(char *x, char *y, int ky){
    for(int i=0; i<ky; i++){
        x[i]=y[i];
        if(islower(x[i])){
            x[i]-=32;
            }
        }
    printf("Using initialised message\n\n");
}

void scan_code(char *x, int k){
    printf("Input message to code/decode: ");
    char in;
    for(int n=0; n<k; n++){ 
        scanf("%c",&in);
        x[n]=in;
        if(islower(x[n])){
            x[n]-=32;
        }
        if(x[n]==10){
            n=k;
            break;
        }
    }
    printf("\n");
}

void print_code(char *x, int k, int ky){
    if(ky!=1){
        k=ky;
    }
    for(int i=0; i<k; i++){
        printf("%c",x[i]);
    }
    printf("\n\n");
}

void encode_caesar(char *x, int k, int ky, int key){
    if(ky!=1){
        k=ky;
    }
    if(key==0){
        printf("Select key to use to encode: ");
        scanf("%d",&key);
    }
    for(int n=0; n<k; n++){
        if(isupper(x[n])){
            x[n]=(((x[n]-65)+key)%26)+65;
        }
    }
}

void decode_caesarwkey(char *x, int k, int ky, int key){
    if(ky!=1){
        k=ky;
    }
    if(key==0){
        printf("Select key to use to decode: ");
        scanf("%d",&key);
    }
    for(int n=0; n<k; n++){
        if(isupper(x[n])){
            x[n]=((26+(x[n]-65)-key)%26)+65;
        }
    }
}

void decode_caesarwokey(char*x, char *z, int k, int ky){
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