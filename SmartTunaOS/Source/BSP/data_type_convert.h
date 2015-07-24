/*
*write by wsg 2015-1-7
*/
int char2int(unsigned char a_char,unsigned char a_flag);
void one2two(unsigned char *data_init,unsigned char *data_a,unsigned char *data_b);
void two2one(unsigned char *data_inits,unsigned char *data_aa,unsigned char *data_bb);
void one2three(unsigned char *data_init,unsigned char *data_a,unsigned char *data_b,unsigned char *data_c);
void three2one(unsigned char *data_inits,unsigned char *data_aa,unsigned char *data_bb,unsigned char *data_cc);
void one2six(unsigned char *data_init,unsigned char *data_a,unsigned char *data_b,unsigned char *data_c,unsigned char *data_d,unsigned char *data_e,unsigned char *data_f);
void six2one(unsigned char *data_inits,unsigned char *data_aa,unsigned char *data_bb,unsigned char *data_cc,unsigned char *data_dd,unsigned char *data_ee,unsigned char *data_ff);
unsigned char* Hex2String(unsigned char *pSrc,unsigned char nL); 
unsigned char change_flag(unsigned char d_c);
unsigned char change_dd(signed char d_c);