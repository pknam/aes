#include <iostream>
#include <fstream>
#include "gf.h"
#include "aes.h"

using namespace std;

int main()
{
	unsigned char block[16];
	unsigned char key[16];

	fstream f_plain;
	fstream f_cipher;
	ifstream f_key;

	char select;

	cout << "Operation (e/d) : ";
	cin >> select;

	switch (select)
	{
	case 'e':
	case 'E':
		f_plain.open("plain.bin", fstream::binary | fstream::in);
		f_key.open("key.bin", fstream::binary | fstream::in);
		f_cipher.open("cipher.bin", fstream::binary | fstream::out | fstream::trunc);

		f_plain.read((char*)block, 16);
		f_key.read((char*)key, 16);
		aes_enc(block, key);			// ENCRYPTION
		f_cipher.write((char*)block, 16);

		break;

	case 'd':
	case 'D':
		f_plain.open("plain2.bin", fstream::binary | fstream::out | fstream::trunc);
		f_key.open("key.bin", fstream::binary | fstream::in);
		f_cipher.open("cipher.bin", fstream::binary | fstream::in);

		f_cipher.read((char*)block, 16);
		f_key.read((char*)key, 16);
		aes_dec(block, key);			// DECRYPTION
		f_plain.write((char*)block, 16);

		break;

	default:
		cout << "Zzz" << endl;
		return -1;
	}

	f_plain.close();
	f_key.close();
	f_cipher.close();

	return 0;
}