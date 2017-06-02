#include <stdio.h>
#include <stdlib.h>
#include "simulator.h"

int mem_out(int addr, int range){
    if(addr < 0 || addr + range < 0 ||addr > 1023 || addr + range > 1023){
        halt = 1;
        return 1;
    }
    return 0;
}

void PC_overflow(){
    if(PC < 0 || PC >= 256){
        halt = 1;
    }
}
