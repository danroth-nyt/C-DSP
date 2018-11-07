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

#ifndef __afir__
#define __afir__

#include "audioeffectx.h"
#include "dezip.h"
//$safeprojectname$

//-------------------------------------------------------------------------------------------------------
class aFIR : public AudioEffectX
{
public:
	aFIR (audioMasterCallback audioMaster);
	~aFIR ();

	// Processing
	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
	virtual void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames);

	// Program
	virtual void setProgramName (char* name);
	virtual void getProgramName (char* name);

	// Parameters
	virtual void setParameter (VstInt32 index, float value);
	virtual float getParameter (VstInt32 index);
	virtual void getParameterLabel (VstInt32 index, char* label);
	virtual void getParameterDisplay (VstInt32 index, char* text);
	virtual void getParameterName (VstInt32 index, char* text);

	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion ();

protected:
	DeZipper dezipL, dezipR;
	float fFilter;
	const int IRLen = 48;
	float xbufL[48], xbufR[48];
	int offsetL = 0, offsetR = 0;
	enum {kFilter, knumParams};
	char programName[kVstMaxProgNameLen + 1];
};

#endif
