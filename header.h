#ifndef BurrowsWheeler
#define BurrowsWheeler

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fstream>

typedef unsigned char byte;

using namespace std;
using namespace System;

unsigned FileSize(ifstream *file);
byte* ReadBlock(ifstream *in, unsigned N);
void WriteBlock(ofstream *out, byte *outbuf, unsigned N);

//------------------ RLE Encoding -----------------------------------//
byte* RLE_encode (byte *input, unsigned N, unsigned &out);
byte* RLE_decode (byte *input, unsigned N, unsigned &out);

//------------------ Burrowsa-Wheeler Transform -----------------//
int compare(const int *left, const int *right);
byte* BWT_encode(byte *input, unsigned N, unsigned &index);
byte* BWT_decode(byte *input, unsigned N, unsigned index);

//------------------ Move-To-Front Transform ----------------------------------//
void MTF_init ();
byte* MTF_encode (byte *input, unsigned N);
byte* MTF_decode (byte *input, unsigned N);

//------------------ Arithmetic encoder ----------------------------//
void Arit_encode (byte *input, unsigned N);
byte* Arit_decode (unsigned N);

void Arit_encode_start(ofstream *file);
void Arit_encode_stop();
void Arit_decode_start(ifstream *file);

#endif