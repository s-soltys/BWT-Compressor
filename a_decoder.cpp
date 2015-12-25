#include <stdafx.h>
#include "a_header.h"

ifstream *input;
int in_buff, in_bits, garbage_bits;

inline int get_bit()
{   
    if (in_bits==0) { 
        in_buff = input->get();
        in_bits = 8;
    }
    int t = in_buff&1; 
    in_buff >>= 1; 
    in_bits--;

    return t;
}

static long low, high, value, length, symbol, cum;
int decode_symbol( int cumul_freq[] )
{
    length = high-low+1;
    cum = ((value-low+1)*cumul_freq[0]-1)/length;
    for (symbol = 1; cumul_freq[symbol]>cum; symbol++) ;
    high = low -1 + (length*cumul_freq[symbol-1])/cumul_freq[0];
    low = low + (length*cumul_freq[symbol])/cumul_freq[0];

    for (;;) {                             
        if (high<q2);
        else if (low>=q2) {
            value -= q2;
            low -= q2;
            high -= q2;
        }
        else if (low>=q1 && high<q3) {
            value -= q1;
            low -= q1;
            high -= q1;
        }
        else break;

        low = 2*low;
        high = 2*high+1;
        value = 2*value+get_bit();
    }
    return (symbol-1);
}

void Arit_decode_start(ifstream *file)
{
	input = file;
	init_model();

    in_bits = 0;

    value = 0; 
    for (int i = 1; i<=16; i++) { 
        value = 2*value + get_bit();
    }
    low = 0; 
    high = top;
}

byte* Arit_decode(unsigned N)
{
	unsigned x=0;
	byte *output=new byte[N]; 

	if(!input->is_open()){
		Console::WriteLine("Blad podczas wczytywania pliku");
		Console::Read();
		exit(-1);
	}

	for (unsigned i=0 ; i<N ; i++) { 
        int symbol;
        symbol = decode_symbol(cumul_freq);  
		if (symbol==EOF) break; 

        output[x++]=(unsigned char) symbol; 
        update_model(symbol);                  
    }

	return output;
}