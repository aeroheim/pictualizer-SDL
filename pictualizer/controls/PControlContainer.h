#pragma once

#include <vector>
#include "PControl.h"
#include "PControlContainerStates.h"

class PControlContainer : public PControl
{
	public:
		PControlContainer(float x, float y, float w, float h);
		~PControlContainer();

		void setElementIndex(int index);
		int getElementIndex();
		int getNumElements();

		void addElement(PControl* element);
		void insertElement(int index, PControl* element);
		void removeElement(int index);

		void setElement(int index, PControl*);
		PControl* getElement(int index);

		void setTransitionState(PControlContainerTransitionState ts);
		PControlContainerTransitionState getTransitionState();

		void setX(float x);
		void setY(float y);
		void setWidth(float width);
		void setHeight(float height);

		void setColor(float r, float g, float b);
		void setAlpha(float a);

		void draw(SDL_Renderer* ren);

	private:
		PControlContainerTransitionState transitionState;
		std::vector<PControl*> elements;
		int elementIndex;

		void setElementDimensions(PControl* element);
		void setAllElementDimensions();
};