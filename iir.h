//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:27 $
//
// Category     : VST 2.x SDK Samples
// Filename     : afir.h
// Created by   : Steinberg Media Technologies
// Description  : Stereo plugin which applies Gain [-oo, 0dB]
//
// © 2006, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#ifndef __aiir__
#define __aiir__

#include "audioeffectx.h"

#define host2param(a,b,v) v*(b-a)+a
#define param2host(a,b,v) (v-a)/(b-a)

#define FCMAX 7000.f
#define FCMIN 50.f
#define QMAX 10.f
#define QMIN 0.1f

#define _USE_MATH_DEFINES
//$safeprojectname$

//-------------------------------------------------------------------------------------------------------
class aIIR : public AudioEffectX
{
public:
	aIIR(audioMasterCallback audioMaster);
	~aIIR();

	// Processing
	virtual void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames);
	virtual void processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames);

	// Program
	virtual void setProgramName(char* name);
	virtual void getProgramName(char* name);

	// Parameters
	virtual void setParameter(VstInt32 index, float value);
	virtual float getParameter(VstInt32 index);
	virtual void getParameterLabel(VstInt32 index, char* label);
	virtual void getParameterDisplay(VstInt32 index, char* text);
	virtual void getParameterName(VstInt32 index, char* text);

	virtual bool getEffectName(char* name);
	virtual bool getVendorString(char* text);
	virtual bool getProductString(char* text);
	virtual VstInt32 getVendorVersion();
	virtual void resume();
	virtual void setSampleRate(float sampleRate);
	

protected:
	void coefCalc32(), coefCalc64();
	float fCutoff, fQ, fS;
	float B0, B1, B2, A0, A1, A2;
	float z1, z2;
	double dblB0, dblB1, dblB2, dblA0, dblA1, dblA2;
	double dz1, dz2;
	enum { kCutoff, kQ, knumParams };
	char programName[kVstMaxProgNameLen + 1];
};

#endif


