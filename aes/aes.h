#pragma once

// initialize sbox, inverse sbox
void sbox_init();
unsigned char sbox(unsigned char in);
unsigned char inv_sbox(unsigned char in);

// prev_key : 직전의 키
// times : 구할 round key가 몇번재 round key인지 (0..9)
void get_next_roundkey(unsigned char* prev_key, int times);
void key_expansion(unsigned char* key);
void add_roudnkey(unsigned char* block, unsigned char* roundkey);

// in encryption
void sub_bytes(unsigned char* block);
void shift_rows(unsigned char* block);
void mix_column(unsigned char* block);

// in decryption
void inv_sub_bytes(unsigned char* block);
void inv_shift_rows(unsigned char* block);
void inv_mix_column(unsigned char* block);

// enc, dec func
void aes_enc(unsigned char* block, unsigned char* key);
void aes_dec(unsigned char* block, unsigned char* key);