#include <stdafx.h>
#include "header.h"

//---------------------------  Funkcja zwracaj¹ca rozmiar pliku ----------------------------//
unsigned FileSize(ifstream *file)
{
	file->seekg( 0, ios::end );
    unsigned N = file->tellg();
    file->seekg( 0, ios::beg );
	return N;
}

//---------- Funkcja wczytuj¹ca blok o okreœlonym rozmiarze z pliku --------------------//
byte* ReadBlock(ifstream *in, unsigned N)
{
	ifstream *input = in;
	if(!input->is_open()){
		Console::WriteLine("Blad podczas wczytywania pliku");
		Console::Read();
		exit(-1);
	}

	byte *inbuf=new byte[N];

	for(unsigned i=0; i<N; i++)
		inbuf[i]=input->get();

	return inbuf;
}

//-----  Funkcja zapisuj¹ca blok o okreœlonym rozmiarze do pliku wyjœciowego -----------//
void WriteBlock(ofstream *out, byte *outbuf, unsigned N)
{
	ofstream *output = out;
	if(!output->is_open()){
		Console::WriteLine("Blad podczas zapisywania pliku");
		Console::Read();
		exit(-1);
	}

	for(unsigned i=0; i<N; i++)
		output->put(outbuf[i]);

	delete outbuf;
}