#pragma once
class Vowel {
public:
	Vowel(double Fc[], double Q[], double G[]);
	~Vowel(void);
	double* Fc;
	double* Q;
	double* G;
	//static inrp
	
};

/*
float fCinterPol, QinterPol, GinterPol;
int p = 0;
float aFc[5] = {
	600.0 / m_nSampleRate,
	1040.0 / m_nSampleRate,
	2250.0 / m_nSampleRate,
	2450.0 / m_nSampleRate,
	2750.0 / m_nSampleRate };
float aQ[5] = { 10.0, 14.86, 20.46, 20.42, 21.15 };
float aG[5] = { 0.0, -7.0, -9.0, -9.0, -20.0 };
float Cutoff[5], Q[5], Gain[5];*/