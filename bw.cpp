#include <stdafx.h>
#include "header.h"

unsigned buff_len;
byte *buffer;

//------------------------------------  BWT ------------------------------------------//
inline int compare(const int *left, const int *right)
{
    int l = *left, r = *right, count=buff_len;

	// pêtla porównuj¹ca dwie rotacje
    while (buffer[l] == buffer[r])
    {
		// przesuwanie wskaŸnika wzd³u¿ obu rotacji
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

	// tablica wskaŸników pocz¹tkowych pozycji rotacji danych wejœciowych
    int *pointers = new int[N];
    for(unsigned i=0; i<N; ++i)
        pointers[i] = i;

	// sortowanie rotacji za pomoc¹ funkcji qsort i compare
	qsort(pointers, (int)N, sizeof(int),
		 (int(*)(const void *, const void *)) compare);

	// wyznaczanie ostatniej kolumny posortowanych rotacji oraz wskaŸnika pozycji pocz¹tkowej
	int x=0;
    for (unsigned i=0; i<N; ++i)
    {
		// wydzielanie kolejnych bajtów znajduj¹cych siê w ostatniej kolumnie
		if (pointers[i] != 0)
			output[x++]=input[pointers[i]-1];
        else
			output[x++]=input[N-1];

		// wydzielenie wskaŸnika pozycji pocz¹tkowej
		if (pointers[i] == 0)
			index = i;
    }

	delete input;
	return output;
}

//----------------- Odwrotna transformacja Burrowsa-Wheelera -----------------------//
byte* BWT_decode(byte *input, unsigned N, unsigned index)
{
	// inicjowanie wektora transformacji T i bufora wyjœciowego
	byte *output = new byte[N];
	unsigned *T = new unsigned[N];

	// inicjowanie tabeli kontekstów K
	int count[256]; int K[256];
	for (int i = 0; i < 256; i++)
		count[i]=K[i]=0;

	// obliczanie liczby powtórzeñ ka¿dego symbolu w ci¹gu wejœciowym
	for (unsigned i = 0; i < N; i++)
		count[input[i]]++;

	// wype³nianie tabeli kontekstów K
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

	// odzyskiwanie pierwotnego bloku danych poprzez kolejne przejœcia wektora T
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