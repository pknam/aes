#include <iostream>
#include <cstring>
#include "aes.h"
#include "gf.h"

unsigned char sbox_table[16][16] = { 0 };
unsigned char inv_sbox_table[16][16] = { 0 };

// 0 : key
// 1 ~ 10 : round keys
unsigned char round_keys[11][16];

void sbox_init()
{
	const int c = 0x63;

	for (int x = 0; x < 16; x++)
	{
		for (int y = 0; y < 16; y++)
		{
			int in = (x << 4) | y;
			int result = 0;

			in = gmul_inverse(in);

			for (int i = 0; i < 8; i++)
			{
				int bit = BIT(in, i);

				for (int j = 4; j < 8; j++)
					bit ^= BIT(in, (i + j) % 8);

				bit ^= BIT(c, i);

				result |= bit << i;
			}

			// set tables
			sbox_table[x][y] = result;
			inv_sbox_table[result >> 4][result & 0x0f] = (x << 4) | y;
		}
	}
}

unsigned char sbox(unsigned char in)
{
	return sbox_table[in >> 4][in & 0x0f];
}

unsigned char inv_sbox(unsigned char in)
{
	return inv_sbox_table[in >> 4][in & 0x0f];
}

void get_next_roundkey(unsigned char* prev_key, int times)
{
	int rcon[10] = { 1 };
	unsigned char tmp_key[4] = { 0 };
	int tmp;

	for (int i = 1; i < 10; i++)
	{
		rcon[i] = rcon[i-1] << 1;

		// carry 발생 시 reduction
		if (rcon[i] & 0x100)
			rcon[i] ^= IP;
	}

	// tmp_key에 prev_key에 복사해뒀다가
	// prev_key에 새로운 round key를 넣어줌
	memcpy(tmp_key, &prev_key[12], 1 * 4);

	tmp = tmp_key[0];
	tmp_key[0] = sbox(tmp_key[1]);
	tmp_key[1] = sbox(tmp_key[2]);
	tmp_key[2] = sbox(tmp_key[3]);
	tmp_key[3] = sbox(tmp);

	prev_key[0] ^= tmp_key[0] ^ rcon[times];
	prev_key[1] ^= tmp_key[1];
	prev_key[2] ^= tmp_key[2];
	prev_key[3] ^= tmp_key[3];

	for (int i = 4; i < 16; i++)
		prev_key[i] ^= prev_key[i - 4];
}

void key_expansion(unsigned char* key)
{
	unsigned char tmpkey[16] = { 0 };
	memcpy(tmpkey, key, 1 * 16);
	memcpy(round_keys[0], tmpkey, 1 * 16);

	for (int i = 0; i < 10; i++)
	{
		get_next_roundkey(tmpkey, i);
		memcpy(round_keys[i + 1], tmpkey, 1 * 16);
	}
}

void sub_bytes(unsigned char* block)
{
	for (int i = 0; i < 16; i++)
	{
		block[i] = sbox(block[i]);
	}
}

void shift_rows(unsigned char* block)
{
	unsigned char tmp, tmp_;

	tmp = block[1];
	block[1] = block[5];
	block[5] = block[9];
	block[9] = block[13];
	block[13] = tmp;

	tmp = block[2];
	tmp_ = block[6];
	block[2] = block[10];
	block[6] = block[14];
	block[10] = tmp;
	block[14] = tmp_;

	tmp = block[15];
	block[15] = block[11];
	block[11] = block[7];
	block[7] = block[3];
	block[3] = tmp;
}

void mix_column(unsigned char* block)
{
	unsigned char tmp_col[4] = { 0 };

	for (int i = 0; i < 4; i++)
	{
		memcpy(tmp_col, &block[i * 4], 1 * 4);

		block[i * 4 + 0] = gmul(0x02, tmp_col[0]) ^ gmul(0x03, tmp_col[1]) ^ tmp_col[2] ^ tmp_col[3];
		block[i * 4 + 1] = tmp_col[0] ^ gmul(0x02, tmp_col[1]) ^ gmul(0x03, tmp_col[2]) ^ tmp_col[3];
		block[i * 4 + 2] = tmp_col[0] ^ tmp_col[1] ^ gmul(0x02, tmp_col[2]) ^ gmul(0x03, tmp_col[3]);
		block[i * 4 + 3] = gmul(0x03, tmp_col[0]) ^ tmp_col[1] ^ tmp_col[2] ^ gmul(0x02, tmp_col[3]);
	}
}

void inv_sub_bytes(unsigned char* block)
{
	for (int i = 0; i < 16; i++)
	{
		block[i] = inv_sbox(block[i]);
	}
}

void inv_shift_rows(unsigned char* block)
{
	unsigned char tmp, tmp_;

	tmp = block[13];
	block[13] = block[9];
	block[9] = block[5];
	block[5] = block[1];
	block[1] = tmp;

	tmp = block[2];
	tmp_ = block[6];
	block[2] = block[10];
	block[6] = block[14];
	block[10] = tmp;
	block[14] = tmp_;

	tmp = block[3];
	block[3] = block[7];
	block[7] = block[11];
	block[11] = block[15];
	block[15] = tmp;
}

void inv_mix_column(unsigned char* block)
{
	unsigned char tmp_col[4] = { 0 };

	for (int i = 0; i < 4; i++)
	{
		memcpy(tmp_col, &block[i * 4], 1 * 4);

		block[i * 4 + 0] = gmul(0x0e, tmp_col[0]) ^ gmul(0x0b, tmp_col[1]) ^ gmul(0x0d, tmp_col[2]) ^ gmul(0x09, tmp_col[3]);
		block[i * 4 + 1] = gmul(0x09, tmp_col[0]) ^ gmul(0x0e, tmp_col[1]) ^ gmul(0x0b, tmp_col[2]) ^ gmul(0x0d, tmp_col[3]);
		block[i * 4 + 2] = gmul(0x0d, tmp_col[0]) ^ gmul(0x09, tmp_col[1]) ^ gmul(0x0e, tmp_col[2]) ^ gmul(0x0b, tmp_col[3]);
		block[i * 4 + 3] = gmul(0x0b, tmp_col[0]) ^ gmul(0x0d, tmp_col[1]) ^ gmul(0x09, tmp_col[2]) ^ gmul(0x0e, tmp_col[3]);
	}
}

void add_roudnkey(unsigned char* block, unsigned char* roundkey)
{
	for (int i = 0; i < 16; i++)
		block[i] ^= roundkey[i];
}

void aes_enc(unsigned char* block, unsigned char* key)
{
	sbox_init();
	key_expansion(key);

	add_roudnkey(block, round_keys[0]);

	for (int i = 0; i < 9; i++)
	{
		sub_bytes(block);
		shift_rows(block);
		mix_column(block);
		add_roudnkey(block, round_keys[i + 1]);
	}

	sub_bytes(block);
	shift_rows(block);

	add_roudnkey(block, round_keys[10]);
}

void aes_dec(unsigned char* block, unsigned char* key)
{
	sbox_init();
	key_expansion(key);

	add_roudnkey(block, round_keys[10]);

	for (int i = 8; i >= 0; i--)
	{
		inv_shift_rows(block);
		inv_sub_bytes(block);
		add_roudnkey(block, round_keys[i + 1]);
		inv_mix_column(block);
	}

	inv_shift_rows(block);
	inv_sub_bytes(block);
	add_roudnkey(block, round_keys[0]);
}