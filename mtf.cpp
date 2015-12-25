#include <stdafx.h>
#include "header.h"

// tablica zawieraj¹ca aktualny ranking symboli
byte state[256];

//---------------- funkcja inicjuj¹ca listê rankingow¹ symboli ----------------------------//
void MTF_init()
{
	for(int i=0; i <256; ++i)
        state[i] = i;
}

//--------------------------- funkcja koduj¹ca MTF --------------------------------------//
byte* MTF_encode (byte *input, unsigned N)
{
	byte *output = new byte[N];
	int i=0;

    for (unsigned index=0; index<N; index++) 
	{
		// wyszukiwanie bie¿¹cego symbolu wewn¹trz listy, wartoœci¹ kodow¹ jest wyszukany indeks
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

//----------------- fukcja dekoduj¹ca dane MTF  ------------------------------//
byte* MTF_decode (byte *input, unsigned N)
{
	byte *output = new byte[N];
	int i=0;

    for (unsigned index=0; index<N; index++) 
	{
		// dekodowana wartoœc znajduje siê w liœcie pod indeksem równym wartoœci wejœciowej
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