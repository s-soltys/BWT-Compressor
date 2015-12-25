#ifndef AritHeader
#define AritHeader

#include <fstream>

typedef unsigned char byte;

using namespace std;
using namespace System;

//--------------- arithmetic encoder parameters --------------------//
#define q1 16384 
#define q2 32768
#define q3 49152
#define top 65535

//---------------------------- adaptive model parameters --------------------//
#define Max_cumul_frequency 16383
extern int cumul_freq[257];

void init_model();
void update_model(int symbol);

#endif