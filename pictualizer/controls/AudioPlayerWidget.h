#pragma once

#include "PWidget.h"
#include "IndexWidget.h"
#include "ImageButton.h"
#include "SeekBar.h"
#include "../util/PUtils.h"

class AudioPlayerWidget : public PWidget
{
	public:
		AudioPlayerWidget(SDL_Renderer* ren, float x, float y, float w, float h);
		~AudioPlayerWidget();

		void setX(float x);
		void setY(float y);
		void setWidth(float w);
		void setHeight(float h);

		void draw(SDL_Renderer* ren = nullptr);

		void handleEvent(Event* e);

	private:
		SDL_Renderer* ren;
		GridPanel playerControlGrid;
		GridPanel bodyGrid;
		GridPanel rightGrid;
		GridPanel bottomGrid;
		GridPanel volButtonGrid;
};