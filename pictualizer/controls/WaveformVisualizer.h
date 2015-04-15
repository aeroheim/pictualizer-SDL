#pragma once

#include <SDL.h>
#include <bass.h>
#include <vector>
#include "PControl.h"

class WaveformVisualizer : public PControl
{
	public:
		WaveformVisualizer(SDL_Renderer* ren, float x, float y, float w, float h);
		~WaveformVisualizer();

		void setStream(HSTREAM* stream);
		HSTREAM* getStream() const;

		void setMsOffset(int ms);
		int getMsOffset() const;

		void setWidth(float w);

		void draw(SDL_Renderer* ren = nullptr);

	private:
		HSTREAM* stream;
		std::vector<float> samples;
		int msOffset;
		int sampleUnit;

		void setSamples();
};