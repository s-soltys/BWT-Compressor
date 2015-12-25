#include <stdafx.h>
#include "header.h"

byte state[256];

void MTF_init()
{
	for(int i=0; i <256; ++i)
    {
        state[i] = i;
    }
}

byte* MTF_encode (byte *input, unsigned N)
{
	byte *output = new byte[N];
	int i=0;

    for (unsigned index=0; index<N; index++) 
	{
		for ( i=0; i<256; i++)
        {
			if(state[i] == input[index]) break;
        }
        
		output[index]=i;

		while(i)
        {
            state[i]=state[i-1];
			i--;
		}
        
        state[0] = input[index];
    }

	delete input;
	return output;
}

byte* MTF_decode (byte *input, unsigned N)
{
	byte *output = new byte[N];
	int i=0;

    for (unsigned index=0; index<N; index++) 
	{
		i = input[index];
        output[index] = state[i];

		while(i)
        {
			state[i] = state[i-1];
			i--;
		}
        
        state[0] = output[index];
    }

	delete input;
	return output;
}