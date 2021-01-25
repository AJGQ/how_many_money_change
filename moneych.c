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

void remove_constant(long* num, long c){
    for(int i = 0; i < LEN; i++){
        num[i] -= c*den[i];
    }
}

void divide_by_x(long* num){
    for(int i = 1; i < LEN; i++){
        num[i-1] = num[i];
    }
    num[LEN-1] = 0;
}

long step(long* num){
    //constant
    long c = num[0];
    remove_constant(num, c);
    divide_by_x(num);
    return c;
}

int main(int argc, char** argv){

    int n_steps = argc == 2 ? atoi(argv[1])+1 : 0;
    long num[LEN] = {1, 0};

    for(int stp = 0; stp < n_steps; stp++){
        step(num);
    }

    printf("%ld\n", step(num));

    return 0;
}
