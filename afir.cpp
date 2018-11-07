//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:27 $
//
// Category     : VST 2.x SDK Samples
// Filename     : aFIR.cpp
// Created by   : Steinberg Media Technologies
// Description  : Stereo plugin which applies Gain [-oo, 0dB]
//
// © 2006, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#include "afir.h"
#include "coeff.h"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new aFIR (audioMaster);
}

//-------------------------------------------------------------------------------------------------------
aFIR::aFIR (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, 1)	// 1 program, 1 parameter only
{
	setNumInputs (2);		// stereo in
	setNumOutputs (2);		// stereo out
	setUniqueID ('AFIR');	// identify
	canProcessReplacing ();	// supports replacing output
	canDoubleReplacing ();	// supports double precision processing

	fFilter = 1.f;			// default to 0 dB
	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
	fillarray();
}

//-------------------------------------------------------------------------------------------------------
aFIR::~aFIR ()
{
	// nothing to do here
}

//-------------------------------------------------------------------------------------------------------
void aFIR::setProgramName (char* name)
{
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void aFIR::getProgramName (char* name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void aFIR::setParameter (VstInt32 index, float value)
{
	fFilter = value;
}

//-----------------------------------------------------------------------------------------
float aFIR::getParameter (VstInt32 index)
{
	return fFilter;
}

//-----------------------------------------------------------------------------------------
void aFIR::getParameterName (VstInt32 index, char* label)
{
	vst_strncpy (label, "Filter", kVstMaxParamStrLen);
}

//-----------------------------------------------------------------------------------------
void aFIR::getParameterDisplay (VstInt32 index, char* text)
{
	dB2string (fFilter, text, kVstMaxParamStrLen);
}

//-----------------------------------------------------------------------------------------
void aFIR::getParameterLabel (VstInt32 index, char* label)
{
	vst_strncpy (label, "dB", kVstMaxParamStrLen);
}

//------------------------------------------------------------------------
bool aFIR::getEffectName (char* name)
{
	vst_strncpy (name, "aFIR", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool aFIR::getProductString (char* text)
{
	vst_strncpy (text, "aFIR", kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------
bool aFIR::getVendorString (char* text)
{
	vst_strncpy (text, "DanwichHorror", kVstMaxVendorStrLen);
	return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 aFIR::getVendorVersion ()
{ 
	return 1000; 
}

//-----------------------------------------------------------------------------------------
void aFIR::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	// filter variables

	
	float y = 0.f;

    while (--sampleFrames >= 0)
    {
		//Left Channel

		xbufL[offsetL] = *in1++;
		y = 0.f;
		for (int i = 0; i < IRLen; i++)
		{
			y = y + coef[i] * xbufL[offsetL++];
			if (offsetL > IRLen - 1)
				offsetL = 0;
		}
		offsetL--;
		if (offsetL < 0)
			offsetL = IRLen - 1;
		*out1++ = y * fFilter;


		//Right Channel

		xbufR[offsetR] = *in2++;
		y = 0.f;
		for (int i = 0; i < IRLen; i++)
		{
			y = y + coef[i] * xbufR[offsetR++];
			if (offsetR > IRLen - 1)
				offsetR = 0;
		}
		offsetR--;
		if (offsetR < 0)
			offsetR = IRLen - 1;
		*out2++ = y * fFilter;
    }
}

//-----------------------------------------------------------------------------------------
void aFIR::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	double dFilter = fFilter;

	// filter variables

	double y = 0.f;

	while (--sampleFrames >= 0)
	{
		//Left Channel

		xbufL[offsetL] = *in1++;
		y = 0.f;
		for (int i = 0; i < IRLen; i++)
		{
			y = y + coef[i] * xbufL[offsetL++];
			if (offsetL > IRLen - 1)
				offsetL = 0;
		}
		offsetL--;
		if (offsetL < 0)
			offsetL = IRLen - 1;
		*out1++ = y * dFilter;

		//Right Channel

		xbufR[offsetR] = *in2++;
		y = 0.f;
		for (int i = 0; i < IRLen; i++)
		{
			y = y + coef[i] * xbufR[offsetR++];
			if (offsetR > IRLen - 1)
				offsetR = 0;
		}
		offsetR--;
		if (offsetR < 0)
			offsetR = IRLen - 1;
		*out2++ = y * dFilter;
	}
}