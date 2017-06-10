#include <stdio.h>
#include <stdlib.h>
#include "simulator.h"

int TLB(int a, int b, int ID) { //TLB [entry] [0:valid, 1:vpn, 2:ppn, 3:LRU]
    if (ID) return I_TLB[a][b];
    else return D_TLB[a][b];
}

int PT(int a, int b, int ID) {  // PTE [entry] [0:valid, 1:ppn, 2:LRU]
    if (ID) return I_PT[a][b];
    else return D_PT[a][b];
}

int Cache(int a, int b, int c, int ID) { // Cache [index] [set] [0:valid,1:tag,2:MRU]
    if (ID) return I_cache[a][b][c];
    else return D_cache[a][b][c];
}

void setTLB(int a, int b, int value, int ID) {
    if (ID) I_TLB[a][b] = value;
    else D_TLB[a][b] = value;
}

void setPT(int a, int b, int value, int ID) {
    if (ID) I_PT[a][b] = value;
    else D_PT[a][b] = value;
}

void setCache(int a, int b, int c, int value, int ID) {
    if (ID) I_cache[a][b][c] = value;
    else D_cache[a][b][c] = value;
}

void updateTLB(int ID, int VPN, int PPN) {
    int i, index = 0, size = (ID) ? I_TLB_size : D_TLB_size;
    for (i = 0; i < size; ++i) {
        if (!TLB(i, 0, ID)) {
            index = i;
            break;
        }
        if (TLB(i, 3, ID) < TLB(index, 3, ID)) index = i;
    }
    setTLB(index, 0, 1, ID);
    setTLB(index, 1, VPN, ID);
    setTLB(index, 2, PPN, ID);
    setTLB(index, 3, Cycle, ID);
}

int searchTLB(int VPN, int TLB_size, int page_size, int offset, int ID) {
    int i, j;
    for (i = 0; i < TLB_size; ++i) { //search in TLB
        if (TLB(i, 0, ID) && TLB(i, 1, ID) == VPN) {
            setTLB(i, 3, Cycle, ID);
            if (ID) {ITLBh++; itlb = 1;} else {DTLBh++; dtlb = 1;}
            return TLB(i, 2, ID) * page_size + offset;
        }
    }
    return -1;
}

int searchPT(int VPN, int page_size, int offset, int ID) {
    if (PT(VPN, 0, ID)) {
        if (ID) IPTh++; else DPTh++;
        updateTLB(ID, VPN, PT(VPN, 1, ID));
        return PT(VPN, 1, ID) * page_size + offset;
    }
    return -1;
}

void updateMRU(int index, int set, int ID) {
    int i, asso, mru_num, cv_num;
    asso = (ID) ? I_cache_asso : D_cache_asso;
    if (asso != 1) {
        mru_num = (ID) ? I_mru_num[index] : D_mru_num[index];
        cv_num = (ID) ? I_cv_num[index] : D_cv_num[index];
        if (!Cache(index, set, 2, ID)) mru_num++;
        if (mru_num == asso && cv_num == asso) {
            for (i = 0; i < asso; ++i) setCache(index, i, 2, 0, ID);
            if (ID) I_mru_num[index] = 0; else D_mru_num[index] = 0;
        }
        if (ID && !Cache(index, set, 2, ID)) I_mru_num[index]++;
        else if (!ID && !Cache(index, set, 2, ID)) D_mru_num[index]++;
        setCache(index, set, 2, 1, ID);
    }
}

void goCMP(int VA, int ID) {
    int i, j, VPN, PPN, offset, TLB_size, page_size, mem_entry, PTE, tag, PA, num, index, asso, block, cache_index, cv_num;
    TLB_size = (ID) ? I_TLB_size : D_TLB_size;
    page_size = (ID) ? I_page_size : D_page_size;
    mem_entry = (ID) ? I_mem_entry : D_mem_entry;
    PTE = (ID) ? I_PTE : D_PTE;
    VPN = VA / page_size;
    offset = VA % page_size;

    PA = searchTLB(VPN, TLB_size, page_size, offset, ID);
    if (PA == -1) {
        if (ID) ITLBm++; else DTLBm++;
        PA = searchPT(VPN, page_size, offset, ID);
        if (PA == -1) {
            if (ID) {IPTm++; idisk = 1;} else {DPTm++; ddisk = 1;}
            num = index = 0;
            for (i = 0; i < PTE; ++i) if (PT(i, 0, ID)) {index = i; break;}
            for (i = 0; i < PTE; ++i) {
                if (PT(i, 0, ID)) {
                    num++;
                    if (PT(i, 2, ID) < PT(index, 2, ID)) index = i;
                }
            }
            if (num == mem_entry) {
                int pos, cache_asso, cache_index, block;
                pos = PT(index, 1, ID);
                cache_asso = (ID) ? I_cache_asso : D_cache_asso;
                cache_index = (ID) ? I_cache_index : D_cache_index;
                block = (ID) ? I_cache_bsize : D_cache_bsize;
                setPT(index, 0, 0, ID);
                setPT(VPN, 0, 1, ID);
                setPT(VPN, 1, pos, ID);
                setPT(VPN, 2, Cycle, ID);
                for (i = 0 ; i < TLB_size ; ++i) if ( TLB(i, 0, ID) && TLB(i, 1, ID) == index ) { setTLB(i, 0, 0, ID); break;}      
                updateTLB(ID, VPN, pos);
                for (i = pos * page_size; i < (pos + 1) * page_size; ++i) { // update Cache
                    tag = (i/block) / cache_index;
                    index = (i/block) % cache_index;
                    for (j = 0; j < cache_asso; ++j) {
                        if (Cache(index, j, 0, ID) && Cache(index, j, 1, ID) == tag) {
                            setCache(index, j, 0, 0, ID);
                            if (ID) I_cv_num[index]--; else D_cv_num[index]--;
                        }
                    }
                }
                PA = pos * page_size + offset;
            }
            else {
                setPT(VPN, 0, 1, ID);
                setPT(VPN, 1, num, ID);
                setPT(VPN, 2, Cycle, ID);
                updateTLB(ID, VPN, num);
                PA = num * page_size + offset;
            }
        }
    }
// Cache
    asso = (ID) ? I_cache_asso : D_cache_asso;
    cache_index = (ID) ? I_cache_index : D_cache_index;
    block = (ID) ? I_cache_bsize : D_cache_bsize;
    tag = (PA / block) / cache_index;
    index = (PA / block) % cache_index;
    cv_num = (ID) ? I_cv_num[index] : D_cv_num[index];

    for (i = 0; i < asso; ++i) {
        if (Cache(index, i, 0, ID) && Cache(index, i, 1, ID) == tag) {
            if (ID) {ICh++; icache = 1;} else {DCh++; dcache = 1;}
            updateMRU(index, i, ID);
            return;
        }
    }

    if (ID) ICm++; else DCm++;
    if (cv_num == asso) {
        for (i = 0; i < asso; ++i) {
            if (!Cache(index, i, 2, ID)) {
                setCache(index, i, 1, tag, ID);
                updateMRU(index, i, ID);
                return;
            }
        }
    }
    else {
        for (i = 0; i < asso; ++i) {
            if (!Cache(index, i, 0, ID)) {
                if (ID) I_cv_num[index]++; else D_cv_num[index]++;
                setCache(index, i, 0, 1, ID);
                setCache(index, i, 1, tag, ID);
                updateMRU(index, i, ID);
                return;
            }
        }
    }
}