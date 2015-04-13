#include "WaveformVisualizer.h"

WaveformVisualizer::WaveformVisualizer(SDL_Renderer* ren, float x, float y, float w, float h) : 
	PControl(x, y, w, h),
	ren(ren),
	texture(nullptr),
	stream(nullptr),
	msOffset(0),
	sampleUnit(0)
{
	texture = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, getRoundedWidth(), getRoundedHeight());
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	dest.x = getRoundedX();
	dest.y = getRoundedY();
	dest.w = getRoundedWidth();
	dest.h = getRoundedHeight();
}

WaveformVisualizer::~WaveformVisualizer()
{
	if (texture)
		SDL_DestroyTexture(texture);
}

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

void WaveformVisualizer::setX(float x)
{
	PControl::setX(x);
	dest.x = getRoundedX();
}

void WaveformVisualizer::setY(float y)
{
	PControl::setY(y);
	dest.y = getRoundedY();
}

void WaveformVisualizer::setWidth(float w)
{
	PControl::setWidth(w);
	dest.w = getRoundedWidth();

	// Sample unit depends on width, so we must adjust it.
	sampleUnit = (int) std::floor(samples.size() / getWidth());

	// Recreate the texture since dimensions have changed.
	if (texture)
	{
		SDL_DestroyTexture(texture);
		texture = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, getRoundedWidth(), getRoundedHeight());
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	}
}

void WaveformVisualizer::setHeight(float h)
{
	PControl::setHeight(h);
	dest.h = getRoundedHeight();

	// Recreate the texture since dimensions have changed.
	if (texture)
	{
		SDL_DestroyTexture(texture);
		texture = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, getRoundedWidth(), getRoundedHeight());
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	}
}

void WaveformVisualizer::setColor(float r, float g, float b)
{
	PControl::setColor(r, g, b);

	if (texture)
	{
		PIntColor color = getRoundedColor();
		SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
	}
}

void WaveformVisualizer::setAlpha(float a)
{
	PControl::setAlpha(a);

	if (texture)
		SDL_SetTextureAlphaMod(texture, getRoundedAlpha());
}

void WaveformVisualizer::draw(SDL_Renderer* ren)
{
	PControl::draw(this->ren);

	if (stream)
	{
		SDL_SetRenderTarget(ren, texture);

		// Clear previous frame.
		SDL_RenderClear(ren);

		// Query the current frame's audio samples.
		BASS_ChannelGetData(*stream, &samples[0], samples.size() * 4);

		// Store the renderer's previous color and set it to the visualizer's colors.
		Uint8 prevR, prevG, prevB, prevA;
		SDL_GetRenderDrawColor(ren, &prevR, &prevG, &prevB, &prevA);
		PIntColor color = getRoundedColor();
		SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, getRoundedAlpha());

		// Draw the points that make up the waveform visualization.
		std::vector<SDL_Point> points;
		int midline = (int) std::round(getRoundedHeight() / 2.0f);

		for (int i = 0; i < getRoundedWidth(); i++)
			points.push_back({ i, midline + (int) std::round(samples[i * sampleUnit] * midline) });

		SDL_RenderDrawLines(ren, &points[0], getRoundedWidth() - 1);

		// Restore the renderer's previous color.
		SDL_SetRenderDrawColor(ren, prevR, prevG, prevB, prevA);

		SDL_SetRenderTarget(ren, nullptr);

		SDL_RenderCopy(ren, texture, nullptr, &dest);
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
