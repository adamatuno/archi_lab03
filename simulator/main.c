#include <stdio.h>
#include <stdlib.h>
#include "simulator.h"

unsigned int readfile(FILE *f) {
    unsigned int a;
    fread(&a, 4, 1, f);
    return (a&0x000000ff)<<24|(a&0x0000ff00)<<8|(a&0x00ff0000)>>8|(a&0xff000000)>>24;
}

int main(int argc, char **argv){
    int i, j;
    unsigned int code;
    Cycle = 1;
    init(argc, argv);
    cycle_0();
    while (!halt && Cycle <= 500000) {
        goCMP(PC * 4, 1);
        code = I[PC];
        PC++;
        if ((code & 0xfc0fffff) != 0x00000000) switch (type(get_op(code))) {
            case 'R':
                Rti(get_func(code), get_rs(code), get_rt(code), get_rd(code), get_sha(code));
                break;
            case 'J':
                JSti(get_op(code), get_addr(code));
                break;
            default:
                Iti(get_op(code), get_rs(code), get_rt(code), get_imm(code));
                break;
        }
        if (!halt) snap();
        trace(code);
        ++Cycle;
    }
    report();
return 0;
}
