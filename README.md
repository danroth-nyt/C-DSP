# IIR Vowel Formant Filter

This C++ Vowel Formant Filter consists of a series of IIR filters in different configurations for testing effective formant synthesis using the source/model theory.  The plugin has filter presets for the 5 basic vowels (a, e, i, o, u) in 3 octaves (bass, tenor, and alto).  The formant filters can be placed in either cascade or parallel and the number of active formants can altered from 0 to 5 filters.  The code is meant to accompany my paper [Proposal for Research, Design, and Evaluation of a Formant Filter](https://sway.office.com/6rPoUHP4RIM2MkBz?ref=Link).

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. 

### Prerequisites

* A DAW that can read VST plugins
* A sound source such as the 200 Hz buzz tone that is provided here for speech synthesis emulation.
* RackAFX (optional)

### Installing

* Method 1: Place the FormantFilter.dll into the VST plugin folder used by your DAW and scan for new plugins.  Open it up as an effect on an audio track and either use the 200 Hz buzz source provided or your own sound material.

* Method 2: The code can be placed into a RackAFX project and a sound source can be imported and tested by compiling inside the development environment.

## Running the tests

Test different vowels, use different combinations of filter quantity and cascade or parallel filter configurations.  Different octaves can be tested to examine the effects of reducing or increasing number of filters or configuration.

## Built With

* [RackAFX](http://www.willpirkle.com/rackafx/) - Will Pirkle's audio IDE 
* [Biquad C++ Code](http://www.earlevel.com/main/2012/11/26/biquad-c-source-code/) - Source code for generating and processing biquad IIR filters

  Redmon, N. (2012) Biquad C++ source code. Available at: http://www.earlevel.com/main/2012/11/26/biquad-c-source-code/ (Accessed: 28     November, 2018).

## Versioning

Version 1.0

### Current Concerns/Issues

* A major element that is missing is a way to implement linear interpolation, although this proved difficult as there is no immediately intuitive time domain representation in the process of audio filtering.  Research has shown that linear interpolation is the simplest and most effective way to interpolate between speech vowels.

* Code cleanup and optimization

## Authors

* **Dan Roth** - *Initial work* 

## Acknowledgments

* Dr. Bruce Wiggins for his help and support on this project.
* Jordan Schnur for his guidance through some of the project code.

