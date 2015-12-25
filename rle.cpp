#include <stdafx.h>
#include "header.h"

int step=3; // RLE encoding threshold

byte* RLE_encode (byte *input, unsigned N, unsigned &out)
{
	unsigned index=out=0;
	byte sign;
	byte count;
	byte *output = new byte[1500000];

	while(index < N)
	{
		// read first symbol and add it to output
		sign = input[index];
		output[out++] = sign;
		count = 1;

		// count number of symbol repetitions
		while((index+1 < N) && (input[index] == input[index+1]))
		{
			if(count==255) break; 
			if(count<step) output[out++]=sign;
			count++; index++;
		} 
		index++;

		// send number of repetitions to output
		if(count>=step) 
			output[out++] = count-step;
	}
	delete input;
	return output;
}

byte* RLE_decode (byte *input, unsigned N, unsigned &out)
{
	unsigned index=out=0;
	byte sign;
	byte count;
	byte *output = new byte[1500000];

	while(index < N)
	{
		// read first symbol and send it to output
		sign = input[index];
		output[out++]=sign;
		count = 1;

		// count number of symbol repetitions
		while((index+1 < N) && (input[index] == input[index+1]))
		{
			count++; index++;
			output[out++]=sign;
			if(count==step) 
				break;
		}
        
		index++;

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