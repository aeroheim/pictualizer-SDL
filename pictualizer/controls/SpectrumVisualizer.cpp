#include "SpectrumVisualizer.h"

SpectrumVisualizer::SpectrumVisualizer(float x, float y, float w, float h) :
	PControl(x, y, w, h),
	stream(nullptr),
	barWidth(1),
	dividerWidth(1),
	globalMod(35)
{
	for (int i = 0; i < MAX_SAMPLES; i++)
		samples[i] = 0;
}

SpectrumVisualizer::~SpectrumVisualizer() {}

void SpectrumVisualizer::setStream(HSTREAM* stream)
{
	this->stream = stream;
}

HSTREAM* SpectrumVisualizer::getStream() const
{
	return stream;
}

void SpectrumVisualizer::addBin(int minFreq, int maxFreq, float mod)
{
	insertBin(bins.size(), minFreq, maxFreq, mod);
}

void SpectrumVisualizer::insertBin(int index, int minFreq, int maxFreq, float mod)
{
	assert(index <= (int) bins.size());

	bins.insert(bins.begin() + index, FrequencyBin{ 0, 0, minFreq, maxFreq, mod });
	setBinNumBars();
}

void SpectrumVisualizer::removeBin(int index)
{
	bins.erase(bins.begin() + index);
	setBinNumBars();
}

void SpectrumVisualizer::setBin(int index, int minFreq, int maxFreq, float mod)
{
	assert(index >= 0 && index < (int) bins.size());

	bins[index].minFreq = minFreq;
	bins[index].maxFreq = maxFreq;
	bins[index].mod = mod;

	bins[index].freqUnit = (int) std::round((maxFreq - minFreq) / (float) bins[index].numBars);
}

FrequencyBin SpectrumVisualizer::getBin(int index) const
{
	assert(index >= 0 && index < (int) bins.size());

	return bins[index];
}

void SpectrumVisualizer::setBarWidth(float bw)
{
	barWidth = bw;
	setBinNumBars();
}

float SpectrumVisualizer::getBarWidth() const
{
	return barWidth;
}

void SpectrumVisualizer::setDividerWidth(float dw)
{
	dividerWidth = dw;
	setBinNumBars();
}

void SpectrumVisualizer::setWidth(float w)
{
	PControl::setWidth(w);
	setBinNumBars();
}

void SpectrumVisualizer::draw(SDL_Renderer* ren)
{
	PControl::draw(ren);

	if (stream)
	{
		if (BASS_ChannelIsActive(*stream) == BASS_ACTIVE_PLAYING)
			BASS_ChannelGetData(*stream, samples, BASS_DATA_FFT16384);
		else
		{
			// Smoothly fade out the samples when the stream has stopped.
			for (int i = 0; i < MAX_SAMPLES; i++)
				samples[i] = 0 * (1 - SMOOTH_CONSTANT) + samples[i] * (SMOOTH_CONSTANT);
		}
	}

	// Store the renderer's previous color and set it to the visualizer's colors.
	Uint8 prevR, prevG, prevB, prevA;
	SDL_GetRenderDrawColor(ren, &prevR, &prevG, &prevB, &prevA);
	PIntColor color = getRoundedColor();
	SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, getRoundedAlpha());

	// Get total number of bars.
	int numBars = 0;

	for (size_t i = 0; i < bins.size(); i++)
		numBars += bins[i].numBars;

	SDL_Rect* bars = new SDL_Rect[numBars];

	int barsCreated = 0;

	for (size_t i = 0; i < bins.size(); i++)
		for (int j = 0; j < bins[i].numBars; j++)
		{
			float averageFFT = getAvgFFTSamples(bins[i].minFreq + j * bins[i].freqUnit, bins[i].minFreq + (j + 1) * bins[i].freqUnit);
			int barHeight = std::min(std::max((int)std::round(averageFFT * getRoundedHeight() * globalMod * bins[i].mod), 1), getRoundedHeight());

			bars[barsCreated] = { (int)std::round(getRoundedX() + barsCreated * (barWidth + dividerWidth)), getRoundedY() + getRoundedHeight(), (int)std::round(barWidth), -barHeight };
			barsCreated++;
		}

	SDL_RenderFillRects(ren, bars, numBars);

	// Restore the renderer's previous color.
	SDL_SetRenderDrawColor(ren, prevR, prevG, prevB, prevA);
}

float SpectrumVisualizer::getAvgFFTSamples(int minFreq, int maxFreq) const
{
	float total = 0;

	// Range is from minFreq to maxFreq exclusive.
	for (int i = minFreq; i < maxFreq; i++)
		total += samples[i];

	return total / (maxFreq - minFreq);
}

void SpectrumVisualizer::setBinNumBars()
{
	float numBars = (getWidth() / (barWidth + dividerWidth)) / bins.size();
	float remainder = numBars - (int) numBars;

	for (size_t i = 0; i < bins.size(); i++)
	{
		if (i < remainder * bins.size())
			bins[i].numBars = (int) std::ceil(numBars);
		else
			bins[i].numBars = (int) std::floor(numBars);

		bins[i].freqUnit = (int) std::round((bins[i].maxFreq - bins[i].minFreq) / (float) bins[i].numBars);
	}
}