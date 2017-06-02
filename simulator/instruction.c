#include <stdio.h>
#include <stdlib.h>
#include "simulator.h"
void JSti(unsigned int op, unsigned int C){
    switch(op){
    case 0x02://j
        PC = (((PC << 2) & 0xf0000000) | (C << 2)) >> 2;
        PC_overflow();
        break;
    case 0x03://jal
        r[31] = PC << 2;
        PC = (((PC << 2) & 0xf0000000) | (C << 2)) >> 2;
        PC_overflow();
        break;
    case 0x3f://halt
        halt = 1;
        break;
    default:
        halt = 1;
        printf("illegal instruction found at 0x%08x",PC * 4 - 4);
        break;
    }
}

void Rti(unsigned int func, unsigned int s, unsigned int t, unsigned int d, unsigned int C){
    long long t1, t2, temp;
    unsigned long long ull;
    unsigned int sign;
    int n;
    switch(func){
    case 0x20:///add
        r[d] = r[s] + r[t];
        break;
    case 0x21:///addu
        r[d] = r[s] + r[t];
        break;
    case 0x22:///sub
        r[d] = r[s] - r[t];
        break;
    case 0x24://and
        r[d] = r[s] & r[t];
        break;
    case 0x25://or
        r[d] = r[s] | r[t];
        break;
    case 0x26://xor
        r[d] = r[s] ^ r[t];
        break;
    case 0x27://nor
        r[d] = ~(r[s] | r[t]);
        break;
    case 0x28://nand
        r[d] = ~(r[s] & r[t]);
        break;
    case 0x2a://slt
        r[d] = r[s] < r[t];
        break;
    case 0x00://sll
        r[d] = r[t] << C;
        break;
    case 0x02://srl
        r[d] = (unsigned int)r[t] >> C;
        break;
    case 0x03://sra
        r[d] = r[t] >> C;
        break;
    case 0x08://jr
        PC = r[s] / 4;
        PC_overflow();
        break;
    case 0x18:///mult
        t1 = r[s];
        t2 = r[t];
        ull = t1 * t2;
        Hi = (int) (ull >> 32);
        Lo = (int) (ull & 0x00000000ffffffff);
        break;
    case 0x19:///multu
        t1 = r[s] & 0x00000000ffffffff;
        t2 = r[t] & 0x00000000ffffffff;
        ull = t1 * t2;
        Hi = (int) (ull >> 32);
        Lo = (int) (ull & 0x00000000ffffffff);
        break;
    case 0x10://mfhi
        r[d] = Hi;
        break;
    case 0x12://mflo
        r[d] = Lo;
        r[0] = 0;
        break;
    default:
        halt = 1;
        printf("illegal instruction found at 0x%08x",PC * 4 - 4);
        break;
    }

}

void Iti(unsigned int op, unsigned int s, unsigned int t, int C){
    long long temp;
    unsigned int a, b, c, d, Cu = C & 0x0000ffff;
    switch(op){
    case 0x08://addi
        r[t] = r[s] + C;
        break;
    case 0x09://addiu
        r[t] = r[s] + C;
        break;
    case 0x23://lw
        if(mem_out(r[s] + C, 3)) break;
        goCMP(r[s] + C, 0);
        a = D[r[s] + C] & 0x000000ff;
        b = D[r[s] + C + 1] & 0x000000ff;
        c = D[r[s] + C + 2] & 0x000000ff;
        d = D[r[s] + C + 3] & 0x000000ff;
        r[t] = (a << 24) | (b << 16) | (c << 8) | d;
        break;
    case 0x21://lh
        if(mem_out(r[s] + C, 1)) break;
        goCMP(r[s] + C, 0);
        a = D[r[s] + C];
        b = D[r[s] + C + 1];
        if(a >> 7) r[t] = ((a << 8) | b) | 0xffff0000;
        else r[t] = ((a << 8) | b) & 0x0000ffff;
        break;
    case 0x25://lhu
        if(mem_out(r[s] + C, 1)) break;
        goCMP(r[s] + C, 0);
        a = D[r[s] + C] & 0x000000ff;
        b = D[r[s] + C + 1] & 0x000000ff;
        r[t] = ((a << 8) | b) & 0x0000ffff;
        break;
    case 0x20://lb
        if(mem_out(r[s] + C, 0)) break;
        goCMP(r[s] + C, 0);
        a = D[r[s] + C] & 0x000000ff;
        if(a >> 7) r[t] = a | 0xffffff00;
        else r[t] = a;
        break;
    case 0x24://lbu
        if(mem_out(r[s] + C, 0)) break;
        goCMP(r[s] + C, 0);
        r[t] = D[r[s] + C] & 0x000000ff;
        break;
    case 0x2b://sw
        if(mem_out(r[s] + C, 3)) break;
        goCMP(r[s] + C, 0);
        D[r[s] + C] = (r[t] >> 24) & 0x000000ff;
        D[r[s] + C + 1] = (r[t] >> 16) & 0x000000ff;
        D[r[s] + C + 2] = (r[t] >> 8) & 0x000000ff;
        D[r[s] + C + 3] = r[t] & 0x000000ff;
        break;
    case 0x29://sh
        if(mem_out(r[s] + C, 1)) break;
        goCMP(r[s] + C, 0);
        D[r[s] + C] = (r[t] >> 8) & 0x000000ff;
        D[r[s] + C + 1] = r[t] & 0x000000ff;
        break;
    case 0x28://sb
        if(mem_out(r[s] + C, 0)) break;
        goCMP(r[s] + C, 0);
        D[r[s] + C] = r[t] & 0x000000ff;
        break;
    case 0x0f:///lui
        r[t] = Cu << 16;
        break;
    case 0x0c:///andi
        a = r[s];
        r[t] = a & Cu;
        break;
    case 0x0d:///ori
        a = r[s];
        r[t] = r[s] | Cu;
        break;
    case 0x0e:///nori
        a = r[s];
        r[t] = ~(a | Cu);
        break;
    case 0x0a://slti
        r[t] = r[s] < C;
        break;
    case 0x04://beq
        if(r[s] == r[t]) PC = PC + C;
        break;
    case 0x05://bne
        if(r[s] != r[t]) PC = PC + C;
        break;
    case 0x07://bgtz
        if(r[s] > 0) PC = PC + C;
        break;
    default:
        halt = 1;
        printf("illegal instruction found at 0x%08x",PC * 4 - 4);
        break;
    }
}
