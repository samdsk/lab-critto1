#include <stdio.h>
#include <math.h>
#include <gmp.h>


void RSA_E(
    mpz_t ctxt,
    unsigned long long lm,
    unsigned long long le,
    unsigned long long lp,
    unsigned long long lq  
);

void RSA_D(
    mpz_t m,
    mpz_t ctxt,
    unsigned long long le,
    unsigned long long lp,
    unsigned long long lq  
);

void inv_mul(mpz_t d,mpz_t e, mpz_t p, mpz_t q);

int main(){
    unsigned long long lm = 30120;
    unsigned long long le = 9007;
    unsigned long long lp = 885320963;
    unsigned long long lq = 238855417;

    mpz_t m,ctxt;
    mpz_init(m);
    mpz_init(ctxt);
    RSA_E(ctxt,lm,le,lp,lq);
    RSA_D(m,ctxt,le,lp,lq);

    mpz_clear(ctxt);
    mpz_clear(m);
    return 0;
}

void RSA_E(
    mpz_t ctxt,
    unsigned long long lm,
    unsigned long long le,
    unsigned long long lp,
    unsigned long long lq){ 

    mpz_t m,E,P,Q,n;    
    mpz_inits(m,E,P,Q,n,(mpz_ptr) NULL);

    mpz_set_ui(m,lm);
    mpz_set_ui(E,le);
    mpz_set_ui(P,lp);
    mpz_set_ui(Q,lq);

    mpz_mul(n,P,Q);
    mpz_powm(ctxt,m,E,n);

    printf("ctxt: ");
    mpz_out_str(stdout,10,ctxt);
    printf("\n");

    mpz_clear(m);
    mpz_clear(E);
    mpz_clear(P);
    mpz_clear(Q);
    mpz_clear(n);

    return;
}

void RSA_D(
    mpz_t m,
    mpz_t ctxt,
    unsigned long long le,
    unsigned long long lp,
    unsigned long long lq){

    mpz_t E,P,Q,n,d,phi;    
    mpz_inits(E,P,Q,n,d,phi,(mpz_ptr) NULL);

    mpz_set_ui(E,le);
    mpz_set_ui(P,lp);
    mpz_set_ui(Q,lq);
    mpz_mul(n,P,Q);
  
    mpz_set_ui(d,1);
    mpz_sub_ui(P,P,1);
    mpz_sub_ui(Q,Q,1);    
    mpz_mul(phi,P,Q);

    mpz_invert(d,E,phi);
    mpz_powm(m,ctxt,d,n);

    printf("m: ");
    mpz_out_str(stdout,10,m);
    printf(" d:");
    mpz_out_str(stdout,10,d);
    printf("\n");

    mpz_clear(E);
    mpz_clear(P);
    mpz_clear(Q);
    //mpz_clear(ctxt);
    mpz_clear(n);
    mpz_clear(d);

    return;
}

void inv_mul(mpz_t d,mpz_t e,mpz_t p, mpz_t q){
    
    mpz_t r;
    mpz_t phi;
    //mpz_init(d);
    mpz_init(r);
    mpz_init(phi);
    mpz_set_ui(d,1);
    mpz_sub_ui(p,p,1);
    mpz_sub_ui(q,q,1);
    mpz_mul(phi,p,q);
    

    while(1){
        mpz_mul(r,d,e);
        mpz_mod(r,r,phi);
        if(mpz_cmp_ui(r,1)==0){
            break;
        }
        printf("d: ");
        mpz_out_str(stdout,10,d);
        printf("\n");

        mpz_add_ui(d,d,1);
    }

    mpz_clear(r);
    mpz_clear(phi);
    mpz_clear(p);
    mpz_clear(e);
    mpz_clear(q);

    return;
}