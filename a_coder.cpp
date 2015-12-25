#include <stdafx.h>
#include "a_header.h"

ofstream *output;
int out_buff, out_bits;

inline void send_bit( int bit )
{   
	out_buff >>= 1; 
	if (bit) out_buff |= 0x80;
    out_bits--;

    if (out_bits==0) {
        output->put(out_buff);
        out_bits = 8;
    }
}

static long low, high, opposite_bits, length;   
void encode_symbol(int symbol,int cumul_freq[] )
{   
	length = high-low+1;
    high = low - 1 + (length*cumul_freq[symbol])/cumul_freq[0];
    low = low + (length*cumul_freq[symbol+1])/cumul_freq[0];
    for (;;) {                         
        if (high<q2) {
            send_bit(0);      
			while (opposite_bits>0) {
				send_bit(1);                
				opposite_bits--;                 
			}                     
        }
        else if (low>=q2) {           
            send_bit(1);      
			while (opposite_bits>0) {
				send_bit(0);                
				opposite_bits--;                 
			}       
            low -= q2;
            high -= q2;    
        }
        else if (low>=q1 && high<q3) {
            opposite_bits++;
            low -= q1;
            high -= q1;
        }
        else break;      

        low = 2*low;
        high = 2*high+1;             
    }
}

void Arit_encode_start(ofstream *file){
	output = file;

	if(!output->is_open()){
		Console::WriteLine("Error saving file.");
		Console::Read();
		exit(-1);
	}

	init_model(); 

    out_buff=0; out_bits=8;
    low=opposite_bits=0; high=top;
}

void Arit_encode(byte *input, unsigned N)
{ 
    for (unsigned i=0 ; i<N ; i++) {                
        int symbol;
        symbol = input[i];	                     
  
        encode_symbol(symbol,cumul_freq);      
        update_model(symbol);           
    }
	delete input;
}

void Arit_encode_stop()
{   
	opposite_bits += 1;                        
	if (low<q1)
	{
		send_bit(0);      
		while (opposite_bits>0) {
			send_bit(1);                
			opposite_bits--;                 
		}       
	}
	else {
		send_bit(1);      
		while (opposite_bits>0) {
			send_bit(0);                
			opposite_bits--;                 
		}       
	}
	low=0;
	high=top;

	output->put(out_buff >> out_bits);
}