#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct bitArray{
    char* bits;
    int l;
};
typedef struct bitArray* Bit;

char *sbox1[2][8] = {
    {"101","010","001","110","011","100","111","000"},
    {"001","100","110","010","000","111","101","011"}
};

char *sbox2[2][8] = {
    {"100","000","100","101","111","001","011","010"},
    {"101","011","000","111","110","010","001","100"}
};

Bit initBit(int l){
    Bit i = malloc(sizeof(Bit));
    i->bits = malloc(sizeof(char*)*l+1);
    i->l = l;
    return i;
}

char* XOR(char* a,char* b,int l){
    char* output = malloc(sizeof(char*)*l+1);
    for(int i=0;i<l;i++){
        if(a[i]!=b[i]){
            output[i]='1';
        }else{
            output[i]='0';
        }
    }
    output[l+1]='\0';
    return output;
}

char* E(char* s){
    char *output = malloc(sizeof(char*)*8+1);
    output[0]=s[0];
    output[1]=s[1];
    output[2]=s[3];
    output[3]=s[2];
    output[4]=s[3];
    output[5]=s[2];
    output[6]=s[4];
    output[7]=s[5];
    output[8]='\0';

    return output;
}

char* SBOX(char *sbox[2][8],char*input){
    char fbit=input[0];
    int bit1 = atoi(&fbit);
    int bit234 = (int) strtol(++input,NULL,2);
    return sbox[bit1][bit234];
}

char* keyNTH(char* key,int n){
    char *kn = malloc(sizeof(char*)*8+1);
    for(int i=n-1,count=0;count<8;count++){
        if(i>8) i=0;
        kn[count]=key[i++];        
    }
    kn[8]='\0';
    return kn;
}

char* strcut(char* str,int s,int f){
    char* output = malloc(sizeof(char*)*(f-s)+1);
    int pos=0;
    while(s<f){
        output[pos++]=str[s++];    
    }
    output[pos]='\0';
    return output;
}

char* Feistel(char* R,char* key){
    char* output = malloc(sizeof(char*)*6+1);
    char* Re = E(R);
    Re = XOR(Re,key,8);
    char* s1 = strcut(Re,0,4);
    char* s2 = strcut(Re,4,8);   
    strcat(output,SBOX(sbox1,s1));
    strcat(output,SBOX(sbox2,s2));

    //printf("%s-%s SBOX %s\n",1,s2,output);
    return output;
}

char* DES_4r(char* ptxt,char* key){
    char* L = strcut(ptxt,0,6);
    char* R = strcut(ptxt,6,12);
    //printf("%s%s\n",L,R);

    for(int i=2;i<=4;i++){
        char* temp = Feistel(R,keyNTH(key,i));
        temp = XOR(L,temp,6);
        L=R;
        R=temp;
        //printf("%s%s\n",L,R);
    }
    char* output = malloc(sizeof(char*)*12+1);
    strcat(output,L);
    strcat(output,R);

    output[12]='\0';
    return output;
}

char* att(char* ctxt,char* actxt){
    char* L = strcut(ctxt,0,6);
    char* Ls = strcut(actxt,0,6);

    char* outL = E(XOR(L,Ls,6));
}

int main(){    
    char* ptxt = "000111011011";
    char* key = "001001101";

    char* aptxt = "000000011011";

    char* ctxt = DES_4r(ptxt,key);
    char* actxt = DES_4r(aptxt,key);

    printf("%s -> %s\n",ptxt,ctxt);
    printf("%s -> %s\n",aptxt,actxt);

    return 0;
}