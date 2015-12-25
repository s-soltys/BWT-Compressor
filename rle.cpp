#include <stdafx.h>
#include "header.h"

int step=3; // zmienna definiuj¹ca wartoœæ progu kodowania RLE

//---------------------------  Kodowanie RLE --------------------------------------//
byte* RLE_encode (byte *input, unsigned N, unsigned &out)
{
	unsigned index=out=0; // indeks odczytywanego i zapisywanego symbolu
	byte sign;	//	odczytany symbol
	byte count;	//	licznik odczytanych takich samych symboli
	byte *output = new byte[1500000];

	// g³ówna pêtla funkcji kodowania RLE
	while(index < N)
	{
		// odczytanie pierwszego symbolu i przepisanie go na wyjœcie
		sign = input[index];
		output[out++] = sign;
		count = 1;

		// zliczanie kolejnych powtórzeñ tego samego symbolu
		while((index+1 < N) && (input[index] == input[index+1]))
		{
			if(count==255) break; 
			if(count<step) output[out++]=sign;
			count++; index++;
		} 
		index++;

		// zapisywanie na wyjœcie liczby powtórzeñ danego symbolu
		if(count>=step) 
			output[out++] = count-step;
	}
	delete input;
	return output;
}

//---------------------------  Dekodowanie RLE --------------------------------------//
byte* RLE_decode (byte *input, unsigned N, unsigned &out)
{
	unsigned index=out=0; // indeks odczytywanego i zapisywanego symbolu
	byte sign;	//	odczytany symbol
	byte count;	//	licznik odczytanych takich samych symboli
	byte *output = new byte[1500000];

	// g³ówna pêtla funkcji dekodowania RLE
	while(index < N)
	{
		// odczytanie pierwszego symbolu i przepisanie go na wyjœcie
		sign = input[index];
		output[out++]=sign;
		count = 1;

		// zliczanie kolejnych powtórzeñ tego samego symbolu
		while((index+1 < N) && (input[index] == input[index+1]))
		{
			count++; index++;
			output[out++]=sign;
			if(count==step) 
				break;
		}
		index++;

		// wypisywanie kolejnych powtórzeñ symbolu po odczytaniu licznika powtórzeñ
		if(count==step && index < N)
		{
			byte num = input[index];
			for(byte i=0; i<num; i++) 
				output[out++]=sign;
			index++;
		}
	}
	delete input;
	return output;
}