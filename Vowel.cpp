#include "Vowel.h"

Vowel::Vowel(double Fc[], double Q[], double G[])
{

	this->Fc = Fc;
	this->Q = Q;
	this->G = G;
}

Vowel::~Vowel(void)
{
	delete Fc;
	delete Q;
	delete G;
}
