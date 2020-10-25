#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* String;

struct node{
    String s;
    struct node* next;
};
typedef struct node* List;

List insert(List l, String s){
    List n = malloc(sizeof(List));
    n->s = malloc(sizeof(String)*16);
    n->s = strcat(n->s,s);
    n->next=l;
    //printf("%s done\n",s);
    return n;
}

List deleteNode(List l,String s){
    List current = l;
    List prev = NULL;

    for(;current!=NULL;prev = current, current=current->next) 
        if(strcmp(current->s,s)==0) break;
    if(prev==NULL) return current->next;    
    if(current==NULL) return l;
    prev->next = current->next;
    free(current);
    return l;
}
//SBOX table 1
char sbox1[2][8][4] = {
    {"101","010","001","110","011","100","111","000"},
    {"001","100","110","010","000","111","101","011"}
};
//SBOX table 2
char sbox2[2][8][4] = {
    {"100","000","100","101","111","001","011","010"},
    {"101","011","000","111","110","010","001","100"}
};
//4Bit combinations
char bitcomb[16][5] = {
    "0000", "0001", "0010","0011","0100","0101","0111",
    "1000", "1001", "1010","1011","1100","1101","1111"
};
//XOR of 2 strings of length l
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
//Expanding function of DES from 6bit to 8bit
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
//SBOX of DES, takes sbox table & 4bit string -> 3bit string
char* SBOX(char sbox[2][8][4],char*input){
    char fbit=input[0];
    int bit1 = atoi(&fbit);
    int bit234 = (int) strtol(++input,NULL,2);
    return sbox[bit1][bit234];
}
//Returns nth-key (8bit) of main key (9bit)
char* keyNTH(char* key,int n){
    char *kn = malloc(sizeof(char*)*8+1);
    for(int i=n-1,count=0;count<8;count++){
        if(i>8) i=0;
        kn[count]=key[i++];        
    }
    kn[8]='\0';
    return kn;
}
//String cutter
char* strcut(char* str,int s,int f){
    char* output = malloc(sizeof(char*)*(f-s)+1);
    int pos=0;
    while(s<f){
        output[pos++]=str[s++];    
    }
    output[pos]='\0';
    return output;
}
//DES' Feistel function
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
//4-round DES (Simplified DES)
char* DES_4r(char* ptxt){
    char* L = strcut(ptxt,0,6);
    char* R = strcut(ptxt,6,12);
    //printf("%s%s\n",L,R);
    char* key = "001001101";

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
//Generate possible combinations of the nth-key
List genCombinations(String si,String so,int sbox){
    List l = NULL;
    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            if(strcmp(si,XOR(bitcomb[i],bitcomb[j],4))==0){
                char* sout = NULL;
                if(sbox==0){
                    sout = XOR(SBOX(sbox1,bitcomb[i]),SBOX(sbox1,bitcomb[j]),3);
                }else{                    
                    sout = XOR(SBOX(sbox2,bitcomb[i]),SBOX(sbox2,bitcomb[j]),3);                    
                }

                if(strcmp(sout,so)==0){
                    printf("%s-%s\n",sout,so);
                    String s = malloc(sizeof(String)*16);
                    strcat(s,bitcomb[i]);
                    strcat(s,bitcomb[j]);
                    printf("%d - %.*s|%.*s\n",sbox,4,s,4,s+4);
                    l = insert(l,s);
                }
            }
        }
    }

    return l;
}

List findKey(List l1, List l2){
    List keys = NULL;
    printf("Starting... FindKey\n");
    for(List curr1=l1;curr1!=NULL;curr1=curr1->next){
        for(List curr2=l2;curr2!=NULL;curr2=curr2->next){
            printf("curr1: %s - curr2: %s\n",curr1->s,curr2->s);
            if(strcmp(curr1->s,curr2->s)==0){
                printf("Found a key: %s\n",curr1->s);
                keys = insert(keys,curr1->s);
            }
        }
    }
    return keys;
}
//Attack function
List att(char* ptxt,char* aptxt){

    char* ctxt = DES_4r(ptxt);
    char* actxt = DES_4r(aptxt);

    char* L4 = strcut(ctxt,0,6);
    char* L4s = strcut(actxt,0,6);

    char* L4e = E(L4);
    char* L4se = E(L4s);
    char* L4L4s = E(XOR(L4,L4s,8));

    char* R4 = strcut(ctxt,6,12);
    char* R4s = strcut(actxt,6,12);
    char* R4p = XOR(R4,R4s,6);

    char* L1 = strcut(ptxt,0,6);
    char* L1s = strcut(aptxt,0,6);
    char* L1p = XOR(L1,L1s,6);

    char* R4pL1p = XOR(R4p,L1p,6);
    printf("E(L4 xor L4*) = %s\n",L4L4s);
    printf("R4' xor L1' = %s\n",R4pL1p);

    List l1 = genCombinations(strcut(L4L4s,0,4),strcut(R4pL1p,0,3),0);
    List l2 = genCombinations(strcut(L4L4s,4,8),strcut(R4pL1p,3,6),1);
    
    List kf = NULL, kl = NULL, keys = NULL;

    List current = l1;

    while(current!=NULL){
      kf = insert(kf,XOR(strcut(L4e,0,4),strcut(current->s,0,4),4));
      current = current->next; 
    }

    current = l2;
    while(current!=NULL){
        kl = insert(kl,XOR(strcut(L4e,4,8),strcut(current->s,0,4),4));
        current = current->next; 
    }

    for(List currentf=kf;currentf!=NULL;currentf=currentf->next){
        for(List currentl=kl;currentl!=NULL;currentl=currentl->next){
            String s = malloc(sizeof(String)*16);
            s=strcat(s,currentf->s);
            s=strcat(s,currentl->s);
            //printf("Combinations %s\n",s);
            keys = insert(keys,s);
        }
    }

    return keys;
}

int main(){    
    char* ptxt =  "000111011011";
    char* aptxt = "101110011011";

    char* ctxt = DES_4r(ptxt); 
    char* actxt = DES_4r(aptxt);
    
    printf("%s -> %s\n",ptxt,ctxt);
    printf("%s -> %s\n",aptxt,actxt);

    List l = att(ptxt,"010100011011");
    List ll = att(ptxt,"101010011011");
    List lll = att(ptxt,"011000011011");
    List llll = att(ptxt,"011100011011");
    List key = findKey(l,ll);

    for(List curr1=key;curr1!=NULL;curr1=curr1->next){
        printf("Keys: %s\n",curr1->s);
    }

    key = findKey(key,llll);

    for(List curr1=key;curr1!=NULL;curr1=curr1->next){
        printf("Keys: %s\n",curr1->s);
    }
    //key = findKey(key,llll);
    
    return 0;
}