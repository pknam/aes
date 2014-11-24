/*
 * Operations in Galois Field
*/

#pragma once

// Irreducible Polynomial
// #define IP 0x11B // AES standard
#define IP 0x1E7 // assignment

// t의 n번재 비트를 0또는 1로 리턴
#define BIT(t, n) (((1<<n)&t)>>n)



// 가장 왼쪽에 있는 비트의 위치
int msb(int a);

// add
unsigned char gadd(unsigned char a, unsigned char b);

// multiply
unsigned char gmul(unsigned char a, unsigned char b);

// a = b*(ret) + r
unsigned char gdiv(int a, int b, int* r);

// returns multiplicative inverse in IP
unsigned char gmul_inverse(unsigned char a);