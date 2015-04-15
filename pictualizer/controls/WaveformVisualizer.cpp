#include "WaveformVisualizer.h"

WaveformVisualizer::WaveformVisualizer(SDL_Renderer* ren, float x, float y, float w, float h) : 
	PControl(x, y, w, h),
	ren(ren),
	stream(nullptr),
	msOffset(0),
	sampleUnit(0)
{
}

WaveformVisualizer::~WaveformVisualizer() {}

void WaveformVisualizer::setStream(HSTREAM* stream)
{
	this->stream = stream;

	if (this->stream)
		setSamples();
}

HSTREAM* WaveformVisualizer::getStream() const
{
	return stream;
}

void WaveformVisualizer::setMsOffset(int ms)
{
	msOffset = ms;

	if (stream)
		setSamples();
}

int WaveformVisualizer::getMsOffset() const
{
	return msOffset;
}

void WaveformVisualizer::setWidth(float w)
{
	PControl::setWidth(w);

	// Sample unit depends on width, so we must adjust it.
	sampleUnit = (int) std::floor(samples.size() / getWidth());
}

void WaveformVisualizer::draw(SDL_Renderer* ren)
{
	PControl::draw(this->ren);

	if (stream && getAlpha() != 0)
	{
		// Query the current frame's audio samples.
		BASS_ChannelGetData(*stream, &samples[0], samples.size() * 4);

		// Store the renderer's previous color and set it to the visualizer's colors.
		Uint8 prevR, prevG, prevB, prevA;
		SDL_GetRenderDrawColor(ren, &prevR, &prevG, &prevB, &prevA);
		PIntColor color = getRoundedColor();
		SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, getRoundedAlpha());

		// Draw the points that make up the waveform visualization.
		SDL_Point* points = new SDL_Point[getRoundedWidth()];

		int midline = getRoundedY() + (int) std::round(getRoundedHeight() / 2.0f);

		for (int i = 0; i < getRoundedWidth() - 1; i++)
			points[i] = { getRoundedX() + i, midline + (int)std::round(samples[i * sampleUnit] * (int) std::round(getRoundedHeight() / 2.0f)) };

		SDL_RenderDrawLines(ren, points, getRoundedWidth() - 1);

		delete[] points;

		// Restore the renderer's previous color.
		SDL_SetRenderDrawColor(ren, prevR, prevG, prevB, prevA);
	}
}

void WaveformVisualizer::setSamples()
{
	float sampleRate;
	BASS_ChannelGetAttribute(*stream, BASS_ATTRIB_FREQ, &sampleRate);

	samples.clear();
	samples.resize((int) std::round(sampleRate * (msOffset / 1000.0f)));

	sampleUnit = (int) std::floor(samples.size() / getWidth());
}
