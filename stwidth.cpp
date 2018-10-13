//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:27 $
//
// Category     : VST 2.x SDK Samples
// Filename     : stwidth.cpp
// Created by   : Steinberg Media Technologies
// Description  : Stereo plugin which applies Gain [-oo, 0dB]
//
// © 2006, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#include "StWidth.h"
#include "string"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new StWidth(audioMaster);
}

//-------------------------------------------------------------------------------------------------------
StWidth::StWidth(audioMasterCallback audioMaster)
	: AudioEffectX(audioMaster, 1, knumparams)	// 1 program, 2 parameter only
{
	setNumInputs(2);		// stereo in
	setNumOutputs(2);		// stereo out
	setUniqueID('StWi');	// identify
	canProcessReplacing();	// supports replacing output
	canDoubleReplacing();	// supports double precision processing

	fWidth = 0.f;			// default to 0
	fBal = 0.5f;
	vst_strncpy(programName, "Default", kVstMaxProgNameLen);	// default program name
}

//-------------------------------------------------------------------------------------------------------
StWidth::~StWidth()
{
	// nothing to do here
}

//-------------------------------------------------------------------------------------------------------
void StWidth::setProgramName(char* name)
{
	vst_strncpy(programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void StWidth::getProgramName(char* name)
{
	vst_strncpy(name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void StWidth::setParameter(VstInt32 index, float value)
{
	switch (index)
	{
	case kWidth:
		fWidth = value;
		degrees = fWidth * 90.f;
		break;
	case kBal:
		fBal = value;
		break;
	}
}

//-----------------------------------------------------------------------------------------
float StWidth::getParameter(VstInt32 index)
{
	float ret = 0;
	switch (index)
	{
	case kWidth:
		ret = fWidth;
		break;
	case kBal:
		ret = fBal;
		break;
	}
	return ret;
}

//-----------------------------------------------------------------------------------------
void StWidth::getParameterName(VstInt32 index, char* label)
{
	switch (index)
	{
	case kWidth:
		strcpy(label, " Width ");
		break;
	case kBal:
		strcpy(label, " Balance ");
		break;
	}
}

//-----------------------------------------------------------------------------------------
void StWidth::getParameterDisplay(VstInt32 index, char* text)
{

	std::string s;
	switch (index)
	{
	case kWidth:
		s = std::to_string(degrees);
		vst_strncpy(text, s.c_str(), kVstMaxParamStrLen);
		break;
	case kBal:
		s = std::to_string(fBal * 2.f);
		vst_strncpy(text, s.c_str(), kVstMaxParamStrLen);
		break;
	}
}

//-----------------------------------------------------------------------------------------
void StWidth::getParameterLabel(VstInt32 index, char* label)
{
	
	switch (index)
	{
	case kWidth:
		strcpy(label, " Degrees ");
		break;
	case kBal:
		strcpy(label, " Balance ");
		break;
	}
	
}

//------------------------------------------------------------------------
bool StWidth::getEffectName(char* name)
{
	vst_strncpy(name, "Stereo Width", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool StWidth::getProductString(char* text)
{
	vst_strncpy(text, "Stereo Width", kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------
bool StWidth::getVendorString(char* text)
{
	vst_strncpy(text, "Jor-Dan", kVstMaxVendorStrLen);
	return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 StWidth::getVendorVersion()
{
	return 1001;
}

//-----------------------------------------------------------------------------------------
void StWidth::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float* inLeft = inputs[0];
	float* inRight = inputs[1];
	float* outLeft = outputs[0];
	float* outRight = outputs[1];

	while (--sampleFrames >= 0)
	{
		float Left = (*inLeft++);
		float Right = (*inRight++);
		float Sum = Left + Right;
		float Diff = Left - Right;
		float PhaseX = cos(degrees)*Sum;
		float PhaseY = sin(degrees)*Diff;
		float N__L = PhaseX + PhaseY;
		float N__R = PhaseX - PhaseY;
		(*outLeft++) = (1 - fBal) * N__L;
		(*outRight++) = fBal * N__R;
	}
}

//-----------------------------------------------------------------------------------------
void StWidth::processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames)
{
	double* inLeft = inputs[0];
	double* inRight = inputs[1];
	double* outLeft = outputs[0];
	double* outRight = outputs[1];

	while (--sampleFrames >= 0)
	{
		double Left = (*inLeft++);
		double Right = (*inRight++);
		//double Sum = Left + Right;
		//double Diff = Left - Right;
		double PhaseX = cos(degrees)* (Left + Right);
		double PhaseY = sin(degrees)* (Left - Right);
		(*outLeft++) = (1 - fBal) * (PhaseX + PhaseY);
		(*outRight++) = fBal * (PhaseX - PhaseY);
	}
}
