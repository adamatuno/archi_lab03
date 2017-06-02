#include <stdio.h>
#include <stdlib.h>

fpos_t pos;
int r[32], rl[32], Cycle, err_overwrite_HiLo;
unsigned int Hi, Hil, Lo, Lol, PC, PCin, D[1024], I[256], iin, din, halt, spin;
FILE *ii, *di, *sn, *tr, *re;

unsigned int readfile(FILE *f);
void init(int argc, char **argv);

unsigned int get_op(unsigned int i);
unsigned int get_rs(unsigned int i);
unsigned int get_rt(unsigned int i);
unsigned int get_rd(unsigned int i);
unsigned int get_sha(unsigned int i);
unsigned int get_func(unsigned int i);
int get_imm(unsigned int i);
int get_addr(unsigned int i);
char type(unsigned int op);
long long itl(int a);
int mem_out(int addr, int range);
void Rti(unsigned int func, unsigned int s, unsigned int t, unsigned int d, unsigned int C);
void JSti(unsigned int op, unsigned C);
void Iti(unsigned int op, unsigned int s, unsigned int t, int C);
void cycle_0();
void snap();
void trace();
void report();
void PC_overflow();
//cmp
int TLB(int a, int b, int ID);
int PT(int a, int b, int ID);
int Cache(int a, int b, int c, int ID);
void setTLB(int a, int b, int value, int ID);
void setPT(int a, int b, int value, int ID) ;
void setCache(int a, int b, int c, int value, int ID);
void updateTLB(int ID, int VPN, int PPN);
void updateMRU(int index, int set, int ID);
void goCMP(int VA, int ID);
void printfMIPS(unsigned int code);
int idisk, ddisk, icache, dcache, itlb, dtlb, ipt, dpt;
// parameter
int I_mem_size, D_mem_size, I_page_size, D_page_size, I_cache_tsize, I_cache_bsize, I_cache_asso, D_cache_tsize, D_cache_bsize, D_cache_asso;
// counting hit miss num
int ICh, ICm, DCh, DCm, ITLBh, ITLBm, DTLBh, DTLBm, IPTh, IPTm, DPTh, DPTm;
// cache [index] [set] [0:valid,1:tag,2:MRU]
int I_cache[1024][1024][3], D_cache[1024][1024][3];
int I_cache_index, D_cache_index, I_cache_set, D_cache_set, I_mru_num[1024], D_mru_num[1024], I_cv_num[1024], D_cv_num[1024]; 
// mem
int I_mem_entry, D_mem_entry;
// TLB [entry] [0:valid, 1:vpn, 2:ppn, 3:LRU]
int I_TLB[1024][4], D_TLB[1024][4];
int I_TLB_size, D_TLB_size;
// PTE [entry] [0:valid, 1:ppn, 2:LRU]
int I_PT[1024][3], D_PT[1024][3];
int I_PTE, D_PTE;
