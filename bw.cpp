#include <stdafx.h>
#include "header.h"

unsigned buff_len;
byte *buffer;

inline int compare(const int *left, const int *right)
{
    int l = *left, r = *right, count=buff_len;

    while (buffer[l] == buffer[r])
    {
		l = ++l % buff_len;
		r = ++r % buff_len;

		count--;
        if (!count) break;
    }
	return  buffer[l] - buffer[r];
}

//-------------------- Simple Burrowsa-Wheeler transform function --------------------//
byte* BWT_encode(byte *input, unsigned N, unsigned &index)
{
	byte *output = new byte[N];
	buffer = input;
	buff_len = N;

    int *pointers = new int[N];
    for(unsigned i=0; i<N; ++i)
        pointers[i] = i;

	// sortowanie rotacji za pomoc� funkcji qsort i compare
	qsort(pointers, (int)N, sizeof(int),
		 (int(*)(const void *, const void *)) compare);

	// wyznaczanie ostatniej kolumny posortowanych rotacji oraz wska�nika pozycji pocz�tkowej
	int x=0;
    for (unsigned i=0; i<N; ++i)
    {
		// wydzielanie kolejnych bajt�w znajduj�cych si� w ostatniej kolumnie
		if (pointers[i] != 0)
			output[x++]=input[pointers[i]-1];
        else
			output[x++]=input[N-1];

		// wydzielenie wska�nika pozycji pocz�tkowej
		if (pointers[i] == 0)
			index = i;
    }

	delete input;
	return output;
}

//----------------- Reverse Burrowsa-Wheeler transform -----------------------//
byte* BWT_decode(byte *input, unsigned N, unsigned index)
{
	// init transformation vector T and output buffer
	byte *output = new byte[N];
	unsigned *T = new unsigned[N];

	// init context table K
	int count[256]; int K[256];
	for (int i = 0; i < 256; i++)
		count[i]=K[i]=0;

	for (unsigned i = 0; i < N; i++)
		count[input[i]]++;

	// fill context table K
	K[0] = 0;
	for (int j = 1; j < 256; j++)
		K[j] = K[j-1]+count[j-1];

	// build transform vector T
	for (unsigned i = 0; i < N ; i++)
	{
		byte s = input[i];
		T[ K[s] ]=i;
		K[s]++;
	}

	// restoring the original data block
	int x = index;
	for (unsigned i = 0; i < N; i++)
	{
		x = T[x];
		output[i] = input[x];
		if (x == index)
			break;
	}

	delete input,T;
	return output;
}