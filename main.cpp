#include <stdafx.h>
#include <iostream>
#include <ctime>
#include "header.h"

unsigned block_size = 1000000; // rozmiar bloku transformacji BW

clock_t start, stop;

//--------------------- Funkcja kompresuj¹ca plik danych --------------------------//
void encode(ifstream *InputFile, ofstream *OutputFile)
{
		Console::Clear();
		start = clock(); 
														
		unsigned length, file_size, parts, rest, *indexes, *blocks, rle_size;
		file_size = FileSize(InputFile);
		
		// obliczanie liczby bloków i rozmiaru ostatniego bloku
		parts = file_size/block_size;
		rest = file_size%block_size;

		// jeœli rozmiar ostatniego bloku jest zbyt ma³y, jest on w³¹czany do przedostatniego bloku
		if(rest < 0.2*block_size){
			if(parts==0) parts++;
			else rest+=block_size;
		}
		else parts++;
	
		// wype³nianie tablicy bloków
		indexes = new unsigned[parts];
		blocks = new unsigned[parts];
		for(unsigned i=0; i<parts; i++) blocks[i] = block_size;
		blocks[parts-1]=rest;

		// przesuniêcie wskaŸnika za miejsce zarezerwowane dla nag³ówka pliku skompresowanych danych
		OutputFile->seekp((1+2*parts)*sizeof(unsigned), ios::beg );	
		
		// proces w³aœciwej kompresji pliku
		byte *input, *bwt, *output, *rle;
		Arit_encode_start(OutputFile); MTF_init(); 
		for(unsigned i=0; i<parts; i++)
		{
			length=blocks[i]; 
			
			// odczytywanie bloku
			input = ReadBlock(InputFile,length);									

			// wstêpne kodowanie bloku kodem RLE
			rle = RLE_encode (input, length, rle_size);
			blocks[i]=rle_size;

			// transformacja B-W
			bwt = BWT_encode (rle, rle_size, indexes[i]);

			// kodowanie MTF
			output = MTF_encode (bwt,rle_size);		

			// kodowanie arytmetyczne i zapisywanie do pliku
			Arit_encode(output,rle_size);
		}
		Arit_encode_stop();

		// zapisywanie nag³ówka na pocz¹tku pliku skompresowanych danych
		OutputFile->seekp( 0 , ios::beg );	
		OutputFile->write((char*)&parts,sizeof(unsigned)); 
		for(unsigned i=0; i<parts; i++){
			OutputFile->write((char*)&blocks[i],sizeof(unsigned)); 
			OutputFile->write((char*)&indexes[i],sizeof(unsigned)); 
		}
		stop = clock();
}

//------------------------ Funkcja dekompresuj¹ca --------------------------------//
void decode(ifstream *InputFile, ofstream *OutputFile)
{
		Console::Clear();
		start = clock();
		
		unsigned length, parts, *indexes, *blocks, rle_size;

		// odczytywanie nag³ówka pliku skompresowanych danych
		InputFile->read((char*)&parts,sizeof(unsigned));
		indexes = new unsigned[parts];
		blocks = new unsigned[parts];
		for(unsigned i=0; i<parts; i++){
			InputFile->read((char*)&blocks[i],sizeof(unsigned));
			InputFile->read((char*)&indexes[i],sizeof(unsigned));
		}

		// proces dekompresji pliku wejœciowego
		byte *input, *mtf, *rle, *bwt;
		Arit_decode_start(InputFile); MTF_init();
		for(unsigned i=0; i<parts; i++){
			rle_size = blocks[i];

			// odczytywanie pliku skompresowanego i dekodowanie arytmetyczne
			input = Arit_decode(rle_size);

			// dekodowanie MTF
			mtf = MTF_decode (input,rle_size);

			// odwrotna transformacja BW
			bwt = BWT_decode (mtf, rle_size, indexes[i]);

			// dekodowanie RLE
			rle = RLE_decode(bwt, rle_size, length);

			// zapis do pliku
			WriteBlock(OutputFile,rle,length);
		}
		stop = clock();
}

//---------------------------  MAIN --------------------------------------//
int main(array<System::String ^> ^args)
{
	ifstream *InputFile = new ifstream;  
	ofstream *OutputFile = new ofstream; 
	char *file_in	= new char[30];
	char *file_out	= new char[30];

	cout<<"Kompresor danych wykorzystujacy transformacje Burrowsa-Wheelera\n\n";

	// otwieranie plików
	cout<<"Nazwa pliku wejsciowego:\n";
	cin>>file_in;
	cout<<"Nazwa pliku wyjsciowego:\n";
	cin>>file_out;

	InputFile->open(file_in,ios::binary | ios::in );
	OutputFile->open(file_out,ios::binary | ios::trunc );

	// wybór funkcji programu - kompresja lub dekompresja pliku wejœciowego
	Console::WriteLine("Kompresja/Dekompresja    [k/d]");
	switch ( Console::Read() )
	{
		case 'k':
			encode(InputFile,OutputFile);
			break;
		case 'd':
			decode(InputFile,OutputFile);
			break;
		default:
			Console::WriteLine("Nie wybrano odpowiedniej opcji");
			break;
	}

	// zamykanie otwartych plików
	InputFile->close(); OutputFile->close();

	// obliczanie czasu dzia³ania programu
	double  duration = (double)(stop - start) / CLOCKS_PER_SEC;
	Console::WriteLine("Czas wykonania programu: "+duration+" s");

	Console::WriteLine("Koniec");
	Console::ReadKey();
    return 0;
}