#pragma once

#include "PControlContainer.h"
#include <cassert>

PControlContainer::PControlContainer(float x, float y, float w, float h) :
	PControl(x, y, w, h),
	elementIndex(-1)
{}

PControlContainer::~PControlContainer() {}

void PControlContainer::setDrawElement(int index)
{
	assert(index >= 0 && index < (int) elements.size());

	if (transitionState == PControlContainerTransitionState::FADE)
	{
		// Previous elementIndex may be invalid, such as if a remove was performed.
		if (elementIndex >= 0 && elementIndex < (int) elements.size())
			elements[elementIndex]->setFadeState(PControlFadeState::FADEOUT);

		elements[index]->setFadeState(PControlFadeState::FADEIN);
	}

	elementIndex = index;
}

int PControlContainer::getElementIndex()
{
	return elementIndex;
}

int PControlContainer::getNumElements()
{
	return elements.size();
}

void PControlContainer::addElement(PControl* element)
{
	insertElement(elements.size(), element);
}

void PControlContainer::insertElement(int index, PControl* element)
{
	assert(index <= (int) elements.size());

	if (elementIndex >= index)
		setDrawElement(elementIndex + 1);

	setElementDimensions(element);

	elements.insert(elements.begin() + index, element);
}

void PControlContainer::removeElement(int index)
{
	assert(index >= 0 && index < (int) elements.size());

	if (elementIndex >= index)
		setDrawElement(elementIndex - 1);

	elements.erase(elements.begin() + index);
}

void PControlContainer::replaceElement(int index, PControl* element)
{
	assert(index >= 0 && index < (int) elements.size());

	setElementDimensions(element);

	elements[index] = element;
}

PControl* PControlContainer::getElement(int index)
{
	assert(index >= 0 && index < (int) elements.size());

	return elements[index];
}

void PControlContainer::setTransitionState(PControlContainerTransitionState ts)
{
	transitionState = ts;
}

PControlContainerTransitionState PControlContainer::getTransitionState()
{
	return transitionState;
}

void PControlContainer::setX(float x)
{
	PControl::setX(x);
	setAllElementDimensions();
}

void PControlContainer::setY(float y)
{
	PControl::setY(y);
	setAllElementDimensions();
}

void PControlContainer::setWidth(float w)
{
	PControl::setWidth(w);
	setAllElementDimensions();
}

void PControlContainer::setHeight(float h)
{
	PControl::setHeight(h);
	setAllElementDimensions();
}

void PControlContainer::setColor(float r, float g, float b)
{
	PControl::setColor(r, g, b);

	for (PControl* element : elements)
		element->setColor(r, g, b);
}

void PControlContainer::setAlpha(float a)
{
	PControl::setAlpha(a);

	for (PControl* element : elements)
		element->setAlpha(a);
}

void PControlContainer::draw(SDL_Renderer* ren)
{
	PControl::draw(ren);

	if (elementIndex >= 0 && elementIndex < (int) elements.size())
	{
		if (transitionState == PControlContainerTransitionState::FADE)
		{
			// Draw the current element and all elements that have yet to fade out.
			for (size_t i = 0; i < elements.size(); i++)
			{
				if (i == elementIndex)
					elements[i]->draw(ren);
				else if (elements[i]->getAlpha() != 0)
					elements[i]->draw(ren);
			}
		}
		else
			elements[elementIndex]->draw(ren);
	}
}

void PControlContainer::setElementDimensions(PControl* element)
{
	element->setX(getX());
	element->setY(getY());
	element->setWidth(getWidth());
	element->setHeight(getHeight());
}

void PControlContainer::setAllElementDimensions()
{
	for (PControl* element : elements)
		setElementDimensions(element);
}
