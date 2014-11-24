#include "gf.h"

int msb(int a)
{
	int result = 0;

	for (int i = 0; i < 32; i++)
	{
		if ((1 << i) & a)
			result = i;
	}

	return result;
}

unsigned char gadd(unsigned char a, unsigned char b)
{
	return a ^ b;
}

unsigned char gmul(unsigned char a, unsigned char b)
{
	unsigned char result = 0;
	unsigned char carry;


	for (int i = 0; i < 8; i++)
	{
		if (i != 0)
		{
			carry = b & 0x80;
			b <<= 1;

			if (carry)
				b ^= IP;
		}

		if (a & 1)
		{
			result ^= b;
		}

		a >>= 1;
	}

	return result;
}

unsigned char gdiv(int a, int b, int* r)
{
	unsigned char q = 0;

	while (1)
	{
		int msb_a = msb(a);
		int msb_b = msb(b);

		if (msb_a < msb_b)
			break;

		q |= 1 << (msb_a - msb_b);

		a = a ^ (b << (msb_a - msb_b));
	}

	*r = a;

	return q;
}

unsigned char gmul_inverse(unsigned char a)
{
	int r1 = 0, r2 = IP, r3 = a;
	unsigned char a1 = 0, a2 = 0, a3 = 1;
	unsigned char q = 0;

	if (a == 0)
		return 0;

	while (r3 != 1)
	{
		r1 = r2, r2 = r3;
		a1 = a2, a2 = a3;

		q = gdiv(r1, r2, &r3);
		a3 = gadd(gmul(a2, q), a1);

		//cout << hex << r3 << ' ' << q3 << ' ' << a3 << endl;
	}

	return a3;
}