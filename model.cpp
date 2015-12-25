#include <stdafx.h>
#include "a_header.h"

int cumul_freq[257];	// tablica skumulowanych częstotliwości występowania symboli
double freq[256];		// tablica zawierająca rozkład częstotliwości występowania symboli

// parametry modelu adaptacyjnego
double weight = 0.005;	// waga z jaką kodowane są kolejne napotkane symbole w stosunku do całego zakresu
int steps = 10;			// liczba kroków pomiędzy aktualizacjami tablicy częstotliwości
double range = 16000.0;

// inne zmienne modelu adaptacyjnego zależne od pozostałych parametrów
double reduction = 1-steps*weight;
double increase = weight*range;
int ticker=0; int *buf;

//-------------------------- funcja inicjująca model adaptacyjny kodera arytmetycznego ----------------------------------//
void init_model()
{
	// wypełnianie początkowe tablicy cumul_freq i freq
	for(int i=256; i>=0; i--)
		cumul_freq[i]=256-i;

	for(int i=0; i<256; i++)
		freq[i] = range/256.0;

	// tworzenie bufora przechowującego napotkane symbole pomiędzy aktualizacjami tablicy
	buf = new int[steps];
	for(int i=0; i<steps; i++)
		buf[i] = 0;
}

//--------------------- funkcja aktualizująca model adaptacyjny kodera arytmetycznego -----------------------//
void update_model( int symbol )
{
	// zapisywanie napotkanych symboli do bufora
	ticker=(ticker+1)%steps;
	buf[ticker] = symbol;

	if(ticker != 0) return; 

	// redukowanie wszystkich symboli o zdefiniowaną wartość
	for(int i=0; i<256; i++)
		freq[i] *= reduction;

	// zwiększanie pól odpowiadających ostatnio napotkanym symbolom w buforze
	for(int i=0; i<steps; i++)
		freq[ buf[i] ] += increase;
	
	// wypełnianie tablicy cumul_freq na podstawie tablicy freq
	cumul_freq[256]=0;
	for(int i=255; i>=0; i--){
		cumul_freq[i]=cumul_freq[i+1] + (int)freq[i];
		if(cumul_freq[i]<=cumul_freq[i+1]) cumul_freq[i]=cumul_freq[i+1]+1;
	}
	if(cumul_freq[0]>Max_cumul_frequency) cumul_freq[0]=Max_cumul_frequency;
}