#include <stdafx.h>
#include "a_header.h"

int cumul_freq[257];	// cumulated frequency table
double freq[256];		// symbol histogram (frequency) table

// adaptive model parameters
double weight = 0.005;	// encoding weight for current symbol
int steps = 10;			// frequency table refresh threshold in number of steps
double range = 16000.0;

// auxiliary parameters
double reduction = 1-steps*weight;
double increase = weight*range;
int ticker=0; int *buf;

void init_model()
{
	for(int i=256; i>=0; i--)
		cumul_freq[i]=256-i;

	for(int i=0; i<256; i++)
		freq[i] = range/256.0;

	buf = new int[steps];
	for(int i=0; i<steps; i++)
		buf[i] = 0;
}

// update adaptive model of the Arithmetic encoder
void update_model( int symbol )
{
	// save symbols to buffer
	ticker=(ticker+1)%steps;
	buf[ticker] = symbol;

	if(ticker != 0) return; 

	// reduce all symbols by the reduction coefficient
	for(int i=0; i<256; i++)
		freq[i] *= reduction;

	for(int i=0; i<steps; i++)
		freq[ buf[i] ] += increase;
	
	// fill cumulated frequency table
	cumul_freq[256]=0;
	for(int i=255; i>=0; i--){
		cumul_freq[i]=cumul_freq[i+1] + (int)freq[i];
		if(cumul_freq[i]<=cumul_freq[i+1]) cumul_freq[i]=cumul_freq[i+1]+1;
	}
	if(cumul_freq[0]>Max_cumul_frequency) cumul_freq[0]=Max_cumul_frequency;
}