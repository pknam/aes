/*
 * Operations in Galois Field
*/

#pragma once

// Irreducible Polynomial
// #define IP 0x11B // AES standard
#define IP 0x1E7 // assignment

// t�� n���� ��Ʈ�� 0�Ǵ� 1�� ����
#define BIT(t, n) (((1<<n)&t)>>n)



// ���� ���ʿ� �ִ� ��Ʈ�� ��ġ
int msb(int a);

// add
unsigned char gadd(unsigned char a, unsigned char b);

// multiply
unsigned char gmul(unsigned char a, unsigned char b);

// a = b*(ret) + r
unsigned char gdiv(int a, int b, int* r);

// returns multiplicative inverse in IP
unsigned char gmul_inverse(unsigned char a);