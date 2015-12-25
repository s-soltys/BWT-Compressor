#include <stdafx.h>
#include <iostream>
#include <ctime>
#include "header.h"

unsigned block_size = 1000000; // Block size
clock_t start, stop;

void encode(ifstream *InputFile, ofstream *OutputFile)
{
		Console::Clear();
		start = clock(); 
														
		unsigned length, file_size, parts, rest, *indexes, *blocks, rle_size;
		file_size = FileSize(InputFile);
		
		// Count number of blocks and size of the last block
		parts = file_size/block_size;
		rest = file_size%block_size;

		// if the last block is too small it's added to the last but one block
		if(rest < 0.2*block_size){
			if(parts==0) parts++;
			else rest+=block_size;
		}
		else parts++;
	
		// fill block table
		indexes = new unsigned[parts];
		blocks = new unsigned[parts];
		for(unsigned i=0; i<parts; i++) blocks[i] = block_size;
		blocks[parts-1]=rest;

		// move pointer to the config section of the output file
		OutputFile->seekp((1+2*parts)*sizeof(unsigned), ios::beg );	
		
		// compression loop
		byte *input, *bwt, *output, *rle;
		Arit_encode_start(OutputFile); MTF_init(); 
		for(unsigned i=0; i<parts; i++)
		{
			length=blocks[i]; 
			
			input = ReadBlock(InputFile,length);									

			rle = RLE_encode (input, length, rle_size);
			blocks[i]=rle_size;

			bwt = BWT_encode (rle, rle_size, indexes[i]);

			output = MTF_encode (bwt,rle_size);		

			Arit_encode(output,rle_size);
		}
		Arit_encode_stop();

		// save file info header
		OutputFile->seekp( 0 , ios::beg );	
		OutputFile->write((char*)&parts,sizeof(unsigned)); 
		for(unsigned i=0; i<parts; i++){
			OutputFile->write((char*)&blocks[i],sizeof(unsigned)); 
			OutputFile->write((char*)&indexes[i],sizeof(unsigned)); 
		}
		stop = clock();
}

void decode(ifstream *InputFile, ofstream *OutputFile)
{
		Console::Clear();
		start = clock();
		
		unsigned length, parts, *indexes, *blocks, rle_size;

		// read file header
		InputFile->read((char*)&parts,sizeof(unsigned));
		indexes = new unsigned[parts];
		blocks = new unsigned[parts];
		for(unsigned i=0; i<parts; i++){
			InputFile->read((char*)&blocks[i],sizeof(unsigned));
			InputFile->read((char*)&indexes[i],sizeof(unsigned));
		}

		// decompression loop
		byte *input, *mtf, *rle, *bwt;
		Arit_decode_start(InputFile); MTF_init();
		for(unsigned i=0; i<parts; i++){
			rle_size = blocks[i];

			input = Arit_decode(rle_size);

			mtf = MTF_decode (input,rle_size);

			bwt = BWT_decode (mtf, rle_size, indexes[i]);

			rle = RLE_decode(bwt, rle_size, length);

			WriteBlock(OutputFile,rle,length);
		}
		stop = clock();
}

int main(array<System::String ^> ^args)
{
	ifstream *InputFile = new ifstream;  
	ofstream *OutputFile = new ofstream; 
	char *file_in	= new char[30];
	char *file_out	= new char[30];

	cout<<"Burrowsa-Wheeler Compressor\n\n";

	cout<<"Input file name:\n";
	cin>>file_in;
	cout<<"Output file name:\n";
	cin>>file_out;

	InputFile->open(file_in,ios::binary | ios::in );
	OutputFile->open(file_out,ios::binary | ios::trunc );

	Console::WriteLine("Compression/Decompression    [c/d]");
	switch ( Console::Read() )
	{
		case 'c':
			encode(InputFile,OutputFile);
			break;
		case 'd':
			decode(InputFile,OutputFile);
			break;
		default:
			Console::WriteLine("Incorrect function");
			break;
	}

	InputFile->close();
    OutputFile->close();

	double  duration = (double)(stop - start) / CLOCKS_PER_SEC;
	Console::WriteLine("Execution time: "+duration+" s");

	Console::WriteLine("Complete");
	Console::ReadKey();
    return 0;
}