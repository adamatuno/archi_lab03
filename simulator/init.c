#include <stdio.h>
#include <stdlib.h>
#include "simulator.h"

void init(int argc, char **argv) {
    int i, j;
    unsigned int word;
    halt = 0;
    ii = fopen("iimage.bin", "rb");
    di = fopen("dimage.bin", "rb");
    sn = fopen("snapshot.rpt", "wb");
    tr = fopen("trace.rpt", "wb");
    re = fopen("report.rpt", "wb");

    PCin = PC = readfile(ii) / 4;
    iin = readfile(ii);
    for (i = 0; i < 256; ++i) {
        if (i < PCin + iin && i >= PCin) I[i] = readfile(ii);
        else I[i] = 0x00000000;
    }
    r[29] = rl[29] = spin = readfile(di);
    din = readfile(di);
    for (i = 0; i < din * 4; i += 4) {
        word = readfile(di);
        D[i] = word >> 24;
        D[i + 1] = (word >> 16) & 0x000000ff;
        D[i + 2] = (word >> 8) & 0x000000ff;
        D[i + 3] = word & 0x000000ff;
    }
    for (i = din * 4; i < 1024; ++i) D[i] = 0x00000000;
    for (i = 0; i < 32; ++i) if (i != 29) r[i] = rl[i] = 0x00000000;
    Hi = Hil = 0x00000000;
    Lo = Lol = 0x00000000;

    I_mem_size = 64;
    D_mem_size = 32;
    I_page_size = 8;
    D_page_size = I_cache_tsize = D_cache_tsize = 16;
    I_cache_bsize = I_cache_asso = D_cache_bsize = 4;
    D_cache_asso = 1;
    ICh = ICm = DCh = DCm = ITLBh = ITLBm = DTLBh = DTLBm = IPTh = IPTm = DPTh = DPTm = 0;
    idisk = ddisk = icache = dcache = itlb = dtlb = 0;

    switch(argc) {
        case 11: D_cache_asso = atoi(argv[10]); // 1   4    4
        case 10: D_cache_bsize = atoi(argv[9]); // 4   4    4
        case 9: D_cache_tsize = atoi(argv[8]);  // 16  16   32
        case 8: I_cache_asso = atoi(argv[7]);   // 4   4    8
        case 7: I_cache_bsize = atoi(argv[6]);  // 4   4    4
        case 6: I_cache_tsize = atoi(argv[5]);  // 16  16   64
        case 5: D_page_size = atoi(argv[4]);    // 16  32   64
        case 4: I_page_size = atoi(argv[3]);    // 8   32   128
        case 3: D_mem_size = atoi(argv[2]);     // 32  256  1024
        case 2: I_mem_size = atoi(argv[1]);     // 64  256  512
        default: break;
    }

    // cache
    I_cache_index = I_cache_tsize/I_cache_bsize/I_cache_asso; // 1  1  2
    D_cache_index = D_cache_tsize/D_cache_bsize/D_cache_asso; // 4  1  2
    for (i = 0; i < I_cache_index; ++i) {
        for (j = 0; j < I_cache_asso; ++j) {
            I_cache[i][j][0] = 0; //valid
            I_cache[i][j][2] = 0; //MRU
        }
        I_mru_num[i] = 0;
        I_cv_num[i] = 0;
    }
    for (i = 0; i < D_cache_index; ++i) {
        for (j = 0; j < D_cache_asso; ++j) {
            D_cache[i][j][0] = 0; //valid
            D_cache[i][j][2] = 0; //MRU
        }
        D_mru_num[i] = 0;
        D_cv_num[i] = 0;
    }
    // PTE
    I_PTE = 1024/I_page_size; // 128 32  8 
    D_PTE = 1024/D_page_size; // 64  32  16
    for (i = 0; i < I_PTE; ++i) {
        I_PT[i][0] = 0; // valid
        I_PT[i][2] = 0; // LRU
    }
    for (i = 0; i < D_PTE; ++i) {
        D_PT[i][0] = 0; // valid
        D_PT[i][2] = 0; // LRU
    }
    // TLB
    I_TLB_size = I_PTE/4; // 32  8  2 
    D_TLB_size = D_PTE/4; // 16  8  4
    for (i = 0; i < I_TLB_size; ++i) {
        I_TLB[i][0] = 0; // valid
        I_TLB[i][3] = 0; // LRU
    }
    for (i = 0; i < D_TLB_size; ++i) {
        D_TLB[i][0] = 0; // valid
        D_TLB[i][3] = 0; // LRU
    }
    // mem
    I_mem_entry = I_mem_size/I_page_size; // 8  8  4
    D_mem_entry = D_mem_size/D_page_size; // 2  8  16
}
