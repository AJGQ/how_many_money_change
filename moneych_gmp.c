#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>

#define LEN 192

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

void print(mpz_t* pol){
    for(int i = 0; i < LEN; i++){
        //printf("%d,", pol[i]);
        mpz_out_str(stdout, 10, pol[i]);
        printf(",");
    }
}

void remove_constant(mpz_t* num, mpz_t c){
    mpz_t cden;
    mpz_init (cden);
    for(int i = 0; i < LEN; i++){
        //num[i] -= c*den[i];
        mpz_mul_si (cden, c, den[i]);
        mpz_sub (num[i], num[i], cden);
    }
}

void divide_by_x(mpz_t* num){
    for(int i = 1; i < LEN; i++){
        mpz_set (num[i-1], num[i]);
    }
    mpz_set_ui (num[LEN-1], 0);
}

void step(mpz_t* num, mpz_t val){
    //constant
    mpz_set(val, num[0]);
    remove_constant(num, val);
    divide_by_x(num);
}

int main(int argc, char** argv){

    int n_steps = argc == 2 ? atoi(argv[1])+1 : 0;

    mpz_t val;
    mpz_init(val);

    mpz_t num[LEN];

    //init num
    for(int i = 0; i < LEN; i++){
        mpz_init_set_ui(num[i], i==0 ? 1 : 0);
    }

    for(int stp = 0; stp < n_steps; stp++){
        step(num, val);
    }

    step(num, val);
    mpz_out_str(stdout, 10, val);
    printf("\n");

    return 0;
}
