#include <stdafx.h>
#include "header.h"

// tablica zawieraj�ca aktualny ranking symboli
byte state[256];

//---------------- funkcja inicjuj�ca list� rankingow� symboli ----------------------------//
void MTF_init()
{
	for(int i=0; i <256; ++i)
        state[i] = i;
}

//--------------------------- funkcja koduj�ca MTF --------------------------------------//
byte* MTF_encode (byte *input, unsigned N)
{
	byte *output = new byte[N];
	int i=0;

    for (unsigned index=0; index<N; index++) 
	{
		// wyszukiwanie bie��cego symbolu wewn�trz listy, warto�ci� kodow� jest wyszukany indeks
		for ( i=0; i<256; i++ )
			if(state[i] == input[index]) break;
		output[index]=i;

		// aktualizacja listy - aktualny symbol jest przypisywany do pierwszej pozycji
		while(i){
            state[i]=state[i-1];
			i--;
		}
        state[0] = input[index];
    }

	delete input;
	return output;
}

//----------------- fukcja dekoduj�ca dane MTF  ------------------------------//
byte* MTF_decode (byte *input, unsigned N)
{
	byte *output = new byte[N];
	int i=0;

    for (unsigned index=0; index<N; index++) 
	{
		// dekodowana warto�c znajduje si� w li�cie pod indeksem r�wnym warto�ci wej�ciowej
		i = input[index];
        output[index] = state[i];

		// aktualizacja listy - aktualny symbol jest przypisywany do pierwszej pozycji
		while(i){
			state[i] = state[i-1];
			i--;
		}
        state[0] = output[index];
    }

	delete input;
	return output;
}