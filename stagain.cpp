//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:27 $
//
// Category     : VST 2.x SDK Samples
// Filename     : again.cpp
// Created by   : Steinberg Media Technologies
// Description  : Stereo plugin which applies Gain [-oo, 0dB]
//
// © 2006, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#include "stagain.h"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new StaGain(audioMaster);
}

//-------------------------------------------------------------------------------------------------------
StaGain::StaGain(audioMasterCallback audioMaster)
	: AudioEffectX(audioMaster, 1, knumparams)	// 1 program, 2 parameter only
{
	setNumInputs(2);		// stereo in
	setNumOutputs(2);		// stereo out
	setUniqueID('StGa');	// identify
	canProcessReplacing();	// supports replacing output
	canDoubleReplacing();	// supports double precision processing

	fGainL = 1.f;			// default to 0 dB
	fGainR = 1.f;
	vst_strncpy(programName, "Default", kVstMaxProgNameLen);	// default program name
}

//-------------------------------------------------------------------------------------------------------
StaGain::~StaGain()
{
	// nothing to do here
}

//-------------------------------------------------------------------------------------------------------
void StaGain::setProgramName(char* name)
{
	vst_strncpy(programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void StaGain::getProgramName(char* name)
{
	vst_strncpy(name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void StaGain::setParameter(VstInt32 index, float value)
{
	switch (index)
	{
		case kGainL: fGainL = value;	break;
		case kGainR: fGainR = value;	break;
	}
}

//-----------------------------------------------------------------------------------------
float StaGain::getParameter(VstInt32 index)
{
	float tmp = 0;
	switch (index)
	{
		case kGainL: tmp = fGainL;	break;
		case kGainR: tmp = fGainR;	break;
	}
	return tmp;
}

//-----------------------------------------------------------------------------------------
void StaGain::getParameterName(VstInt32 index, char* label)
{
	switch (index)
	{
		case kGainL: strcpy(label, " Left Gain ");	break;
		case kGainR: strcpy(label, " Right Gain ");	break;
	}
}

//-----------------------------------------------------------------------------------------
void StaGain::getParameterDisplay(VstInt32 index, char* text)
{
	switch (index)
	{
		case kGainL: dB2string(fGainL, text, kVstMaxParamStrLen); break;
		case kGainR: dB2string(fGainR, text, kVstMaxParamStrLen); break;
	}
}

//-----------------------------------------------------------------------------------------
void StaGain::getParameterLabel(VstInt32 index, char* label)
{
	switch (index)
	{
		case kGainL:
		case kGainR: strcpy(label, "dB"); break;
	}
}

//------------------------------------------------------------------------
bool StaGain::getEffectName(char* name)
{
	vst_strncpy(name, "Stereo Gain", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool StaGain::getProductString(char* text)
{
	vst_strncpy(text, "Stereo Gain", kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------
bool StaGain::getVendorString(char* text)
{
	vst_strncpy(text, "Steinberg Media Technologies", kVstMaxVendorStrLen);
	return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 StaGain::getVendorVersion()
{
	return 1000;
}

//-----------------------------------------------------------------------------------------
void StaGain::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float* in1 = inputs[0];
	float* in2 = inputs[1];
	float* out1 = outputs[0];
	float* out2 = outputs[1];

	while (--sampleFrames >= 0)
	{
		(*out1++) = (*in1++) * fGainL;
		(*out2++) = (*in2++) * fGainR;
	}
}

//-----------------------------------------------------------------------------------------
void StaGain::processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames)
{
	double* in1 = inputs[0];
	double* in2 = inputs[1];
	double* out1 = outputs[0];
	double* out2 = outputs[1];
	double dGainL = fGainL;
	double dGainR = fGainR;

	while (--sampleFrames >= 0)
	{
		(*out1++) = (*in1++) * dGainL;
		(*out2++) = (*in2++) * dGainR;
	}
}
