#include <stdio.h>

void scancode(char *x, int k, int ky);

void print_code(char *x, int k, int ky);

void encode_caesar(char *x, int k, int ky);

void decode_caesarwkey(char *x, int k, int ky);

int main()
{
    char in; //what scanf inputs into
    int k,ky,key,select; //k is array size constant, key is what the array is shifted by, select is used to select mode
    char x[1000]; 
    
    //intialised array
    char y[]="OLSSV DVYSK";
    
    k=sizeof(x)/sizeof(char); //finds amount of elements in char which is used as a constant
    
    ky=sizeof(y)/sizeof(char); //elements in intialised array
    
    for(int i=0; i<ky; i++){
        x[i]=y[i];
        if(islower(x[i])){
            x[i]-=32;
        }
    }
    
    print_code(x,k,ky);
    
    
    
    
    
    /*printf("Input message to code/decode: ");
    scancode(x,k);*/
    
    printf("Select Mode\n");
    printf("1=encode using caesar cypher\n");
    printf("2=decode caesar cypher with known key\n");
    scanf("%d",&select);
    
    switch(select){
        case 1:
            encode_caesar(x,k,ky); //call caesar encoder function
            break;
        case 2:
            decode_caesarwkey(x,k,ky); //call caesar decoder function
            break;
    }
    
    print_code(x,k,ky);
    
    

    return 0;
}

void scancode(char *x, int k, int ky){
    char in;
    for(int n=0; n<k; n++){ 
        scanf("%c",&in);
        x[n]=in;
        if(islower(x[n])){
            x[n]-=32;
        }
        if(x[n]==10){
            for(int i=n; i<k; i++){
                x[i]=0;
            }
            break;
        }
    }
    printf("\n");
}

void print_code(char *x, int k, int ky){
    if(ky!=0){
        k=ky;
    }
    for(int i=0; i<k; i++){
        printf("%c",x[i]);
    }
    printf("\n\n");
}

void encode_caesar(char *x, int k, int ky){
    if(ky!=0){
        k=ky;
    }
    int key;
    printf("Select key to use to encode: ");
    scanf("%d",&key);
    for(int n=0; n<k; n++){
        if(isupper(x[n])){
            x[n]=(((x[n]-65)+key)%26)+65;
        }
    }
}

void decode_caesarwkey(char *x, int k, int ky){
    int key;
    if(ky!=0){
        k=ky;
    }
    printf("Select key to use to decode: ");
    scanf("%d",&key);
    for(int n=0; n<k; n++){
        
        if(isupper(x[n])){
            x[n]=(((x[n]-65)-key)%26)+65;
            //printf("%d   %d\n",n,x[n]);
        }
    }
} 