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

		void setX(float x);
		void setY(float y);
		void setWidth(float w);
		void setHeight(float h);

		void setColor(float r, float g, float b);
		void setAlpha(float a);

		void draw(SDL_Renderer* ren = nullptr);

	private:
		SDL_Renderer* ren;
		SDL_Texture* texture;
		SDL_Rect dest;

		HSTREAM* stream;
		std::vector<float> samples;
		int msOffset;
		int sampleUnit;

		void setSamples();
};