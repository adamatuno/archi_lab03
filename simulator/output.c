#include <stdio.h>
#include <stdlib.h>
#include "simulator.h"
void cycle_0() {
    int i;
    fprintf(sn, "cycle 0\n");
    for(i = 0; i < 32; ++i) fprintf(sn, "$%02d: 0x%08X\n", i, r[i]);
    fprintf(sn, "$HI: 0x00000000\n$LO: 0x00000000\nPC: 0x%08X\n\n\n", PC * 4);
}

void snap() {
    int i;
    fprintf(sn, "cycle %d\n", Cycle);
    for(i = 0; i < 32; ++i) {
        if(r[i] != rl[i]) {
            fprintf(sn, "$%02d: 0x%08X\n",i , r[i]);
            rl[i] = r[i];
        }
    }
    if(Hi != Hil) {
        fprintf(sn, "$HI: 0x%08X\n", Hi);
        Hil = Hi;
    }
    if(Lo != Lol) {
        fprintf(sn, "$LO: 0x%08X\n", Lo);
        Lol = Lo;
    }
    fprintf(sn, "PC: 0x%08X\n", PC * 4);
    fprintf(sn, "\n\n");
}

void trace(unsigned int code) {
    fprintf(tr, "%d, ", Cycle);
    printfMIPS(code);
    fprintf(tr, ": ");
    if (idisk) fprintf(tr, "Disk ");
    if (itlb) fprintf(tr, "ITLB ");
    if (icache) fprintf(tr, " ICache ");
    fprintf(tr, " ; ");
    if (ddisk) fprintf(tr, " Disk ");
    if (dtlb) fprintf(tr, " DTLB ");
    if (dcache) fprintf(tr, " DCache ");
    fprintf(tr, "\n");
    idisk = ddisk = icache = dcache = itlb = dtlb = 0;
}

void report() {
    fprintf(re, "ICache :\n# hits: %u\n# misses: %u\n\n", ICh, ICm);
    fprintf(re, "DCache :\n# hits: %u\n# misses: %u\n\n", DCh, DCm);
    fprintf(re, "ITLB :\n# hits: %u\n# misses: %u\n\n", ITLBh, ITLBm);
    fprintf(re, "DTLB :\n# hits: %u\n# misses: %u\n\n", DTLBh, DTLBm);
    fprintf(re, "IPageTable :\n# hits: %u\n# misses: %u\n\n", IPTh, IPTm);
    fprintf(re, "DPageTable :\n# hits: %u\n# misses: %u\n\n", DPTh, DPTm);
}

void printfMIPS(unsigned int code) {
    unsigned int op = get_op(code), fun = get_func(code);
    if(type(op) == 'R') {
        switch(fun){
            case 0x20://add
                fprintf(tr, "add  ");
                break;
            case 0x21://addu
                fprintf(tr, "addu ");
                break;
            case 0x22://sub
                fprintf(tr, "sub  ");
                break;
            case 0x24://and
                fprintf(tr, "and  ");
                break;
            case 0x25://or
                fprintf(tr, "or   ");
                break;
            case 0x26://xor
                fprintf(tr, "xor  ");
                break;
            case 0x27://nor
                fprintf(tr, "nor  ");
                break;
            case 0x28://nand
                fprintf(tr, "nand ");
                break;
            case 0x2a://slt
                fprintf(tr, "slt  ");
                break;
            case 0x00://sll or nop
                if(get_rt(code) == 0 && get_rd(code) == 0 && get_sha(code) == 0) fprintf(tr, "nop  ");//nop
                else fprintf(tr, "sll  "); // sll
                break;
            case 0x02://srl
                fprintf(tr, "srl  ");
                break;
            case 0x03://sra
                fprintf(tr, "sra  ");
                break;
            case 0x08://jr
                fprintf(tr, "jr   ");
                break;
            case 0x18:///mult
                fprintf(tr, "mult ");
                break;
            case 0x19:///multu
                fprintf(tr, "multu");
                break;
            case 0x10://mfhi
                fprintf(tr, "mfhi ");
                break;
            case 0x12://mflo
                fprintf(tr, "mflo ");
                break;
            default:
                break;
            }
    }
    else if(type(op) == 'I') {
        switch(op){
            case 0x08://addi
                fprintf(tr, "addi ");
                break;
            case 0x09://addiu
                fprintf(tr, "addiu");
                break;
            case 0x23://lw
                fprintf(tr, "lw   ");
                break;
            case 0x21://lh
                fprintf(tr, "lh   ");
                break;
            case 0x25://lhu
                fprintf(tr, "lhu  ");
                break;
            case 0x20://lb
                fprintf(tr, "lb   ");
                break;
            case 0x24://lbu
                fprintf(tr, "lbu  ");
                break;
            case 0x2b://sw
                fprintf(tr, "sw   ");
                break;
            case 0x29://sh
                fprintf(tr, "sh   ");
                break;
            case 0x28://sb
                fprintf(tr, "sb   ");
                break;
            case 0x0f:///lui
                fprintf(tr, "lui  ");
                break;
            case 0x0c:///andi
                fprintf(tr, "andi ");
                break;
            case 0x0d:///ori
                fprintf(tr, "ori  ");
                break;
            case 0x0e:///nori
                fprintf(tr, "nori ");
                break;
            case 0x0a://slti
                fprintf(tr, "slti ");
                break;
            case 0x04://beq
                fprintf(tr, "beq  ");
                break;
            case 0x05://bne
                fprintf(tr, "bne  ");
                break;
            case 0x07://bgtz
                fprintf(tr, "bgtz ");
                break;
            default: //wrong
                break;
            }
    }
    else {
        switch(op){
            case 0x02://j
                fprintf(tr, "j    ");
                break;
            case 0x03://jal
                fprintf(tr, "jal  ");
                break;
            case 0x3f://halt
                fprintf(tr, "halt ");
                break;
            default:
                break;
        }
    }
}