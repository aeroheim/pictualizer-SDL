#pragma once

#include "PControl.h"
#include <vector>

class ControlPresenter : public PControl
{
	public:
		void addChild(PControl* child, int zIndex = 0);
		void removeChild(PControl* child);

		void setX(float x) override;
		void setY(float y) override;
		void setWidth(float w) override;
		void setHeight(float h) override;
		void moveTo(float x, float y) override;
		void resize(float w, float h) override;

		void draw(SDL_Renderer* ren) override;

		void notify(Event* e) override;
		void handleEvent(Event* e) override;

	protected:
		ControlPresenter(float x, float y, float w, float h);
		~ControlPresenter();
		std::vector<PControl*> getChildren();

	private:
		std::vector<std::tuple<int, PControl*>> children;
};
