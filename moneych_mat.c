#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define LEN 192
#define LEN2 (LEN*LEN)

/*
 *  A: mat. to exponenciate
 *  R: remainder in R*A^n
 *
 *  R*A^n =
 *  case n=2m:
 *      R*A^2m = R*(A^2)^m
 *  case n=2m+1:
 *      R*A^(2m+1) = (R*A)*(A^2)^m
 *      
 */

mpz_t A[LEN2];
mpz_t R[LEN2];
mpz_t tmp_mult[LEN2];

mpz_t v[LEN];
mpz_t tmp_apply[LEN];

void applyAv(){
    mpz_t tmp;
    mpz_init (tmp);
    for(int i = 0; i < LEN; i++){
        //res[i] = 0;
        mpz_set_si (tmp_apply[i], 0);
        mpz_set_si (tmp, 0);
        int iLEN = i*LEN;
        for(int j = 0; j < LEN; j++){
            //tmp_apply[i] += A[iLEN + j] * v[j];
            mpz_mul (tmp, A[iLEN + j], v[j]);
            mpz_add (tmp_apply[i], tmp_apply[i], tmp);
        }
    }

    for(int i = 0; i < LEN; i++) mpz_set (v[i], tmp_apply[i]);
}

// A = A*B
void mult(mpz_t* A, mpz_t* B){
    mpz_t tmp;
    mpz_init (tmp);
    for(int i = 0; i < LEN; i++){
        int iLEN = i*LEN;
        for(int j = 0; j < LEN; j++){
            //tmp_mult[iLEN + j] = 0;
            mpz_set_si (tmp_mult[iLEN + j], 0);
            for(int k = 0; k < LEN; k++){
                //tmp_mult[iLEN + j] += A[iLEN + k] * B[k*LEN + j];
                mpz_mul (tmp, A[iLEN + k], B[k*LEN + j]);
                mpz_add (tmp_mult[iLEN + j], tmp_mult[iLEN + j], tmp);
            }
        }
    }

    for(int i = 0; i < LEN2; i++) mpz_set (A[i], tmp_mult[i]);
}

// A^1
void expA(mpz_t n){

    // R*A^n
    while(mpz_cmp_si (n, 0) > 0){
        if(mpz_cdiv_ui (n, 2) == 0){
            // R*(A)^2m = R*(A^2)^m
            mult(A, A);
        }else{
            // R*(A)^(2m+1) = (R*A)*(A^2)^m
            mult(R, A);
            mult(A, A);
        }
        mpz_fdiv_q_ui (n, n, 2);
    }
    // R*A^0 = R

    // new A is R
    for(int i = 0; i < LEN2; i++) mpz_set (A[i], R[i]);
}

const long den[LEN] = 
        {1,-1,0,0,0,-1,1,0,0,0,-1,1,0,0,0,1,-1,0,0,0,0,0,0,0,0
        ,-1,1,0,0,0,1,-1,0,0,0,1,-1,0,0,0,-1,1,0,0,0,0,0,0,0,0
        ,-1,1,0,0,0,1,-1,0,0,0,1,-1,0,0,0,-1,1,0,0,0,0,0,0,0,0
        ,1,-1,0,0,0,-1,1,0,0,0,-1,1,0,0,0,1,-1,0,0,0,0,0,0,0,0
        ,-1,1,0,0,0,1,-1,0,0,0,1,-1,0,0,0,-1,1,0,0,0,0,0,0,0,0
        ,1,-1,0,0,0,-1,1,0,0,0,-1,1,0,0,0,1,-1,0,0,0,0,0,0,0,0
        ,1,-1,0,0,0,-1,1,0,0,0,-1,1,0,0,0,1,-1,0,0,0,0,0,0,0,0
        ,-1,1,0,0,0,1,-1,0,0,0,1,-1,0,0,0,-1,1
        };

int main(int argc, char** argv){

    const char* n_steps = argc == 2 ? argv[1] : "1";

    // init A
    for(int i = 0; i < LEN; i++){
        int iLEN = i*LEN;
        for(int j = 0; j < LEN; j++){
            if(j == 0){
                mpz_init_set_si (A[iLEN], i+1==LEN ? 0 : -den[i+1]);
            }else{
                mpz_init_set_si (A[iLEN + j], i+1==j ? 1 : 0);
            }
        }
    }
    
    // init R = Id
    for(int i = 0; i < LEN; i++){
        for(int j = 0; j < LEN; j++){
            mpz_init_set_si (R[i*LEN + j], i==j? 1 : 0);
        }
    }
    
    // init tmp_mult
    for(int i = 0; i < LEN2; i++){
        mpz_init (tmp_mult[i]);
    }
    
    // init v = 1,0,0,...,0
    for(int i = 0; i < LEN; i++){
        mpz_init_set_si (v[i], i==0 ? 1 : 0);
    }

    // init tmp_apply
    for(int i = 0; i < LEN; i++){
        mpz_init (tmp_mult[i]);
    }
    
    mpz_t n;
    mpz_init_set_str (n, n_steps, 10);

    expA(n);
    applyAv();

    mpz_out_str (stdout, 10, v[0]);

    return 0;
}
