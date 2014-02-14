﻿/*
  ==============================================================================

    This file was auto-generated by the Jucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "Knob.h"


//==============================================================================
Knob::Knob()
{
	filterKnobImage = ImageFileFormat::loadFrom (KnobsBI::filterknob_png, KnobsBI::filterknob_pngSize);
	mixKnobImage = ImageFileFormat::loadFrom (KnobsBI::mixknob_png, KnobsBI::mixknob_pngSize);

	//readoutType = "default";
	//testReadout = "o";
	frameWidth = 90;
	//knobType="Filter";

	//Knob::setSliderStyle(RotaryVerticalDrag);
	setSliderStyle(RotaryVerticalDrag);
	//Knob::setText;
	setTextBoxStyle(NoTextBox, true, 0, 0);

	readoutText = "HELLO!!";
}

Knob::~Knob()
{
}

//==============================================================================
void Knob::paint (Graphics& g)
{

	double div;

	// Work out division size
	if (knobType == dualFilterType) div = 1.0 / 62; // 62 frames
	else                            div = 1.0 / 31; // 31 frames

	// Work out current frame number to be shown
	double pos = (int)(getValue() / div);

   // Little hack to get image centred when knob value at half
	if (pos > 0)
		pos = pos - 1;

	// Show last frame
	if (pos == 61)
		pos = 60;

	// Draw Image
	if (knobType == dualFilterType)
		g.drawImage (filterKnobImage, 0,0,90,90, 0, (int)(pos*90), 90, 90, false);
	else if (knobType == mixType)
		g.drawImage (mixKnobImage, 0,0,90,90, 0, (int)(pos*90), 90, 90, false);

	// Draw readout text


	calculateReadoutValue();
	g.setColour(juce::Colour(150,150,150));


	{
		g.setColour(juce::Colour(150,150,150));
		g.drawFittedText(readoutText,0,90,90,50,Justification::centred,1);
	}


}

void Knob::calculateReadoutValue()
{


	 if (knobType == defaultType)
	 readoutText = (String(getValue()));

	 else if (knobType == panType)
	 {
		 if (getValue() < 0.5)
			 readoutText = (String((((getValue() *200.0) -100.0) *-1.0)) + "% L");
		 else if (getValue() == 0.5)
			 readoutText = "C";
		 else if (getValue() > 0.5)
			 readoutText = (String((((getValue() *2.0) -1.0) *100.0)) + "% R");
	 }

	 else if (knobType == gainType)
	 {
		 if (getValue() == 0)
		 {
		 readoutText = "-infdB";
		 //displayText = "-\u221E";
		 }
		 else
		 {
		 //value = value * value * value;
		 readoutText = String(20*log(getValue()));
		 int rounder = readoutText.indexOfChar(0,'.');
		 readoutText = (readoutText.substring(0, rounder+3) + "dB");
		 //displayText = (String((((value *2.0) -1.0) *100.0)) + "% R");
		 }
	 }
	// --------------------------------------------------------------------
	// DUALFILTER TYPE ----------------------------------------------------
	// --------------------------------------------------------------------
	else if (knobType == dualFilterType)
	{
		double newFilterFreq;

		// If dial is in LP mode
		if (getValue() <= 0.5)
		{
			// Work out Frequency (smae as ProcessBlock method)
			newFilterFreq = getValue() * 2.0;								// Scale 0.0-0.5 to 0-1
			newFilterFreq = newFilterFreq * newFilterFreq * newFilterFreq;	// Cube values for smoother control
			newFilterFreq  = (newFilterFreq * 19940.0) + 60;				// Scale to 60Hz to 20000Hz LOWPASS

			// If frequency is in kHz region
			if (newFilterFreq > 1000.0)
			{
				// Convert Hz to kHz
				newFilterFreq = newFilterFreq / 1000.0;
				readoutText = String(newFilterFreq);

				// Round to 1 decimal place
				int rounder = readoutText.indexOfChar('.');
				readoutText = (readoutText.substring(0, rounder+2) + "kHz LP");
			}
			else
				// Output in Hz
				readoutText = String(int(newFilterFreq))+"Hz LP";
		}

		// If dial is in HP mode
		else if (getValue() > 0.5)
		{
			// Work out Frequency (smae as ProcessBlock method)
			newFilterFreq = (getValue() - 0.5) * 2.0;						// Scale 0.5-1.0 to 0-1
			newFilterFreq = newFilterFreq * newFilterFreq * newFilterFreq;	// Cube values for smoother control
			newFilterFreq = (newFilterFreq * 18980.0) + 20;					// 20Hz to 19000Hz HIGHPASS

			// If frequency is in kHz region
			if (newFilterFreq > 1000.0)
			{
				// Convert Hz to kHz
				newFilterFreq = newFilterFreq / 1000.0;
				readoutText = String(newFilterFreq);

				// Round to 1 decimal place
				int rounder = readoutText.indexOfChar('.');

				if (rounder > -1)	readoutText = (readoutText.substring(0, rounder+2) + "kHz HP");
				else				readoutText = "19.0kHz HP";
			}
			else
				// Output in Hz
				readoutText = String(int(newFilterFreq))+"Hz HP";
		}

		// If dial is in centre
		if (getValue() == 0.5 || readoutText == "20Hz HP")
			readoutText = "ALL PASS";
	}
	// --------------------------------------------------------------------
	// --------------------------------------------------------------------


	// --------------------------------------------------------------------
	// MIX TYPE -----------------------------------------------------------
	// --------------------------------------------------------------------
	else if (knobType == mixType)	readoutText = String(int((getValue()*100)))+"%";
	// --------------------------------------------------------------------
	// --------------------------------------------------------------------




	 //return displayText;


}

void Knob::setKnobType (int newType)
{
	knobType = newType;
	repaint();
}

