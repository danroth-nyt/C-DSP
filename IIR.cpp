//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:27 $
//
// Category     : VST 2.x SDK Samples
// Filename     : aIIR.cpp
// Created by   : Steinberg Media Technologies
// Description  : Stereo plugin which applies Gain [-oo, 0dB]
//
// © 2006, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#include "iir.h"
#include "math.h"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new aIIR (audioMaster);
}

//-------------------------------------------------------------------------------------------------------
aIIR::aIIR(audioMasterCallback audioMaster)
	: AudioEffectX(audioMaster, 1, 2)	// 1 program, 1 parameter only
{
	setNumInputs(2);		// stereo in
	setNumOutputs(2);		// stereo out
	setUniqueID('aIIR');	// identify
	canProcessReplacing();	// supports replacing output
	canDoubleReplacing();	// supports double precision processing

	fCutoff = 10000.f;			// default to 7000 Hz
	fQ = 1.f;
	coefCalc32();
	coefCalc64();
	vst_strncpy(programName, "Default", kVstMaxProgNameLen);	// default program name
	z1 = z2 = 0;
	dz1 = dz2 = 0;
}

//-------------------------------------------------------------------------------------------------------
aIIR::~aIIR()
{
	// nothing to do here
}

//-------------------------------------------------------------------------------------------------------
void aIIR::setProgramName(char* name)
{
	vst_strncpy(programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void aIIR::getProgramName(char* name)
{
	vst_strncpy(name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void aIIR::setParameter(VstInt32 index, float value)
{
	switch (index)
	{
	case kCutoff: fCutoff = host2param(FCMIN, FCMAX, value);
		break;
	case kQ: fQ = host2param(QMIN, QMAX, value);
		break;
	}
	coefCalc32();
	coefCalc64(); //Calculate coef 
}

//-----------------------------------------------------------------------------------------
float aIIR::getParameter(VstInt32 index)
{
	float ret = 0.f;
	switch (index)
	{
	case kCutoff: 
		ret = param2host(FCMIN, FCMAX, fCutoff);
		break;
	case kQ: 
		ret = param2host(QMIN, QMAX, fQ);
		break;
	}
	return ret;
}

//-----------------------------------------------------------------------------------------
void aIIR::getParameterName(VstInt32 index, char* label)
{
	switch (index)
	{
	case kCutoff: strcpy(label, " Cutoff ");	
		break;
	case kQ: strcpy(label, " Q ");	
		break;
	}
}

//-----------------------------------------------------------------------------------------
void aIIR::getParameterDisplay(VstInt32 index, char* text)
{
	switch (index)
	{
	case kCutoff:
		float2string(fCutoff, text, kVstMaxParamStrLen);
		break;
	case kQ:
		float2string(fQ, text, kVstMaxParamStrLen);
		break;
	}
}

//-----------------------------------------------------------------------------------------
void aIIR::getParameterLabel(VstInt32 index, char* label)
{
	switch (index)
	{
	case kCutoff: strcpy(label, "Hz");
		break;
	case kQ: strcpy(label, "Q"); 
		break;
	}
}

//------------------------------------------------------------------------
bool aIIR::getEffectName(char* name)
{
	vst_strncpy(name, "aIIR", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool aIIR::getProductString(char* text)
{
	vst_strncpy(text, "aIIR", kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------
bool aIIR::getVendorString(char* text)
{
	vst_strncpy(text, "DanwichHorror", kVstMaxVendorStrLen);
	return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 aIIR::getVendorVersion()
{
	return 1000;
}

void aIIR::resume()
{
	
}

void aIIR::setSampleRate(float sampleRate)
{
	fS = sampleRate;
	coefCalc32();
	coefCalc64(); //Calculate coef 
}

void aIIR::coefCalc32()
{
	//Low Pass
	float w0 = 2.f * M_PI * fCutoff / fS;
	float alpha = sin(w0) / (2.f * fQ);
	float cosw0 = cos(w0);
	A0 = 1.f + alpha;
	B0 = ((1.f - cosw0) / 2.f) / A0;
	B1 = (1.f - cosw0) / A0;
	B2 = ((1.f - cosw0) / 2.f) / A0;
	A1 = (-2.f * cosw0) / A0;
	A2 = (1.f - alpha) / A0;
}

void aIIR::coefCalc64()
{
	double dCutoff = fCutoff;
	double dQ = fQ;

	//Low Pass
	double dw0 = 2.0 * M_PI * dCutoff / fS;
	double dalpha = sin(dw0) / (2.0 * dQ);
	double dcosw0 = cos(dw0);
	dblA0 = 1.0 + dalpha;
	dblB0 = ((1.0 - dcosw0) / 2.0) / dblA0;
	dblB1 = (1.0 - dcosw0) / dblA0;
	dblB2 = ((1.0 - dcosw0) / 2.0) / dblA0;
	dblA1 = (-2.0 * dcosw0) / dblA0;
	dblA2 = (1.0 - dalpha) / dblA0;
}

//-----------------------------------------------------------------------------------------
void aIIR::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float* in1 = inputs[0];
	float* in2 = inputs[1];
	float* out1 = outputs[0];
	float* out2 = outputs[1];
	

	while (--sampleFrames >= 0)
	{
		//Left Channel
		
		float inL = *in1++;
		float outL = (B0 * inL) + z1;
		z1 = (B1 * inL) - (A1 * outL) + z2;	//z1
		z2 = (B2 * inL) - (A2 * outL);		//z2
		*out1++ = outL;
		
		//Right Channel
		float inR = *in2++;
		float outR = B0 * inR + z1;
		z1 = (B1 * inR) - (A1 * outR) + z2;	//z1
		z2 = (B2 * inR) - (A2 * outR);		//z2
		*out2++ = outR;
	}
}

//-----------------------------------------------------------------------------------------
void aIIR::processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames)
{
	double* in1 = inputs[0];
	double* in2 = inputs[1];
	double* out1 = outputs[0];
	double* out2 = outputs[1];
	
	// filter variables


	while (--sampleFrames >= 0)
	{
		//Implement Filter
		//Left Channel
		double inL = *in1++;
		double outL = dblB0 * inL + dz1;
		dz1 = (dblB1 * inL) - (dblA1 * outL) + dz2;	//z1
		dz2 = (dblB2 * inL) - (dblA2 * outL);		//z2
		*out1++ = outL;

		//Right Channel
		double inR = *in2++;
		double outR = dblB0 * inR + dz1;
		dz1 = (dblB1 * inR) - (dblA1 * outR) + dz2;	//z1
		dz2 = (dblB2 * inR) - (dblA2 * outR);		//z2
		*out2++ = outR;
	}
}
