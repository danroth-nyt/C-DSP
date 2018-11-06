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

	fCutoff = 7000.f;			// default to 7000 Hz
	fQ = 0.5f;
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
}

void aIIR::coefCalc32()
{
	//Low Pass
	float w0 = 2.f * M_PI * fCutoff / fS;
	float alpha = sin(w0) / (2.f * fQ);
	float cosw0 = cos(w0);
	B0 = (1.f - cosw0) / 2.f;
	B1 = 1.f - cosw0;
	B2 = 1.f - cosw0 / 2.f;
	A0 = 1.f + alpha;
	A1 = -2.f * cosw0;
	A2 = 1.f - alpha;
}

void aIIR::coefCalc64()
{
	double dCutoff = fCutoff;
	double dQ = fQ;
	//Low Pass
	double dw0 = 2.0 * M_PI * dCutoff / fS;
	double dalpha = sin(dw0) / (2.0 * dQ);
	double dcosw0 = cos(dw0);
	dblB0 = (1.0 - dcosw0) / 2.0;
	dblB1 = 1.0 - dcosw0;
	dblB2 = 1.0 - dcosw0 / 2.0;
	dblA0 = 1.0 + dalpha;
	dblA1 = -2.0 * dcosw0;
	dblA2 = 1.0 - dalpha;
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
		//Implement Filter
		
		float in = *in1++;
		float out = (B0/A0) * in + z1;
		z1 = (B1/A0) * in - (A1/A0)*out + z2;	//z1
		z2 = (B2/A0) * in - (A2/A0)*out;		//z2
		*out1++ = out;
		
		//Right Channel
		*out2++ = *in2++;
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
		double in = *in1++;
		double out = (dblB0/dblA0) * in + dz1;
		dz1 = (dblB1/dblA0) * in - (dblA1/dblA0) * out + dz2;	//z1
		dz2 = (dblB2/dblA0) * in - (dblA2/dblA0) * out;		//z2
		
		*out1++ = out;
		*out2++ = *in2++;
	}
}