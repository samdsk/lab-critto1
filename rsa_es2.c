#include <stdio.h>
#include <math.h>
#include <gmp.h>

const long long unsigned N = 84773093;
const long long unsigned PHI_N = 84754668;

/* 
    Dati n e Phi(n) trovare P e Q.
    Phi(n) = P+Q, n = P*Q. <- sappiamo
    p+q = n-phi(n)+1
    X^2 - Phi(n)*x + n <- eq di sec. grado dove a=1, b= -Phi(n) = -(p+q), c=n. 
    X_1 * X_2 = c = n = pq. <- sappiamo
    X_1 + X_2 = -b = p+q. <- sappiamo

*/

void find_PQ(
    mpz_t p, 
    mpz_t q,
    const long long unsigned ln, 
    const long long unsigned lphi){
    
    mpz_t n, phi,temp1,temp2;
    mpz_init(n);
    mpz_init(phi);
    mpz_init(temp1);
    mpz_init(temp2);
    mpz_set_ui(n,ln);
    mpz_set_ui(phi,lphi);

    mpz_sub(temp1,n,phi);
    mpz_add_ui(temp1,temp1,1);
    mpz_set(temp2,temp1);
    mpz_pow_ui(temp2,temp2,2);
    mpz_sub_ui(temp2,temp2,4*ln);
    mpz_sqrt(temp2,temp2);

    mpz_add(p,temp1,temp2);
    mpz_sub(q,temp1,temp2);
    mpz_cdiv_q_ui(p,p,2);
    mpz_cdiv_q_ui(q,q,2);


    mpz_clear(n);
    mpz_clear(phi);
    mpz_clear(temp1);
    mpz_clear(temp2);

    return;
}


int main(){
    mpz_t p;
    mpz_t q;
    mpz_init(p);
    mpz_init(q);

    find_PQ(p,q,N,PHI_N);

    printf("p: ");
    mpz_out_str(stdout,10,p);
    printf(" q: ");
    mpz_out_str(stdout,10,q);
    printf("\n");

    return 0;
}