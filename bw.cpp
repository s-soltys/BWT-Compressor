#include <stdafx.h>
#include "header.h"

unsigned buff_len;
byte *buffer;

//------------------------------------  BWT ------------------------------------------//
inline int compare(const int *left, const int *right)
{
    int l = *left, r = *right, count=buff_len;

	// p�tla por�wnuj�ca dwie rotacje
    while (buffer[l] == buffer[r])
    {
		// przesuwanie wska�nika wzd�u� obu rotacji
		l = ++l % buff_len;
		r = ++r % buff_len;

		count--;
        if (!count) break;
    }
	return  buffer[l] - buffer[r];
}

//-------------------- Funkcja transformacji prostej Burrowsa-Wheelera --------------------//
byte* BWT_encode(byte *input, unsigned N, unsigned &index)
{
	byte *output = new byte[N];
	buffer = input;
	buff_len = N;

	// tablica wska�nik�w pocz�tkowych pozycji rotacji danych wej�ciowych
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

//----------------- Odwrotna transformacja Burrowsa-Wheelera -----------------------//
byte* BWT_decode(byte *input, unsigned N, unsigned index)
{
	// inicjowanie wektora transformacji T i bufora wyj�ciowego
	byte *output = new byte[N];
	unsigned *T = new unsigned[N];

	// inicjowanie tabeli kontekst�w K
	int count[256]; int K[256];
	for (int i = 0; i < 256; i++)
		count[i]=K[i]=0;

	// obliczanie liczby powt�rze� ka�dego symbolu w ci�gu wej�ciowym
	for (unsigned i = 0; i < N; i++)
		count[input[i]]++;

	// wype�nianie tabeli kontekst�w K
	K[0] = 0;
	for (int j = 1; j < 256; j++)
		K[j] = K[j-1]+count[j-1];

	// budowanie wektora transformacji T
	for (unsigned i = 0; i < N ; i++)
	{
		byte s = input[i];
		T[ K[s] ]=i;
		K[s]++;
	}

	// odzyskiwanie pierwotnego bloku danych poprzez kolejne przej�cia wektora T
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