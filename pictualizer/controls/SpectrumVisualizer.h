#pragma once

#define NOMINMAX

#include <SDL.h>
#include <bass.h>
#include <vector>
#include <algorithm>
#include "PControl.h"

struct FrequencyBin
{
	int numBars;
	int freqUnit;
	int minFreq;
	int maxFreq;
	float mod;
};

class SpectrumVisualizer : public PControl
{
	public:
		SpectrumVisualizer(SDL_Renderer* ren, float x, float y, float w, float h);
		~SpectrumVisualizer();

		void setStream(HSTREAM* stream);
		HSTREAM* getStream() const;

		void addBin(int minFreq, int maxFreq, float mod = 1.0f);
		void insertBin(int index, int minFreq, int maxFreq, float mod = 1.0f);
		void removeBin(int index);

		void setBin(int index, int minFreq, int maxFreq, float mod = 1.0f);
		FrequencyBin getBin(int index) const;
		
		void setBarWidth(float bw);
		float getBarWidth() const;

		void setDividerWidth(float dw);
		float getDividerWidth() const;

		void setWidth(float w);

		void draw(SDL_Renderer* ren = nullptr);

	private:
		SDL_Renderer* ren;

		HSTREAM* stream;

		float barWidth;
		float dividerWidth;

		const float SMOOTH_CONSTANT = 0.9f;

		static const int MAX_SAMPLES = 8192;
		float samples[MAX_SAMPLES];

		std::vector<FrequencyBin> bins;
		float globalMod;

		void setBinNumBars();
		float getAvgFFTSamples(int minFreq, int maxFreq) const;
};