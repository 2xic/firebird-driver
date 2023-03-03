/*
    Helper to look for memory leaks, reuses data from test to make it easy.
*/
#include <stdio.h>
#include "./utils/Profile.h"
//#include "./crypto/Sha1_test_functions.cpp"
#include "./crypto/Srp_test_functions.cpp"

int main(){
    for(int i = 0; i < 100; i++){
        // profile_sha1();
        // TODO: Still some leaks in here, but a lot better
        profile_client_proof();

        if (i % 100 == 0){
            double memory = getMemory();
            printf("memory == %f (%i)\n", memory, i);
        }
    }
    return 0;
}

/*
Reference 
memory == 10348.000000 (0)
memory == 11404.000000 (100)
memory == 12460.000000 (200)
memory == 13384.000000 (300)
memory == 14440.000000 (400)
memory == 15496.000000 (500)
memory == 16556.000000 (600)
memory == 17472.000000 (700)
memory == 18528.000000 (800)
memory == 19584.000000 (900)
memory == 20508.000000 (1000)
memory == 21564.000000 (1100)
memory == 22620.000000 (1200)
memory == 23676.000000 (1300)

---

memory == 10348.000000 (0)
memory == 10348.000000 (100)
memory == 10476.000000 (200)
memory == 10476.000000 (300)
memory == 10476.000000 (400)
memory == 10476.000000 (500)
memory == 10604.000000 (600)
memory == 10604.000000 (700)
memory == 10604.000000 (800)
memory == 10604.000000 (900)
memory == 10732.000000 (1000)
memory == 10732.000000 (1100)
memory == 10732.000000 (1200)
memory == 10732.000000 (1300)
*/
