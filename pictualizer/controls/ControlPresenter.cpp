#pragma once

#include "ControlPresenter.h"
#include <algorithm>
#include <tuple>

void ControlPresenter::addChild(PControl* child, int zIndex = 0)
{
	children.push_back(std::make_tuple(zIndex, child));
	addSubscriber(child);

	// Sort the children by their z-index.
	std::sort(children.begin(), children.end(), [](std::tuple<int, PControl*> a, std::tuple<int, PControl*> b) { return std::get<int>(a) < std::get<int>(b); });
}

void ControlPresenter::removeChild(PControl* child)
{
	children.erase(std::find_if(children.begin(), children.end(), [&child](std::tuple<int, PControl*> element) { return child == std::get<PControl*>(element); }));
	removeSubscriber(child);
}

void ControlPresenter::setX(float x)
{
	PControl::setX(x);
	for (auto& child : children)
	{
		std::get<PControl*>(child)->setX(x);
	}
}

void ControlPresenter::setY(float y)
{
	PControl::setY(y);
	for (auto& child : children)
	{
		std::get<PControl*>(child)->setY(y);
	}
}

void ControlPresenter::setWidth(float w)
{
	PControl::setWidth(w);
	for (auto& child : children)
	{
		std::get<PControl*>(child)->setWidth(w);
	}
}

void ControlPresenter::setHeight(float h)
{
	PControl::setHeight(h);
	for (auto& child : children)
	{
		std::get<PControl*>(child)->setHeight(h);
	}
}

void ControlPresenter::moveTo(float x, float y)
{
	PControl::moveTo(x, y);
	for (auto& child : children)
	{
		std::get<PControl*>(child)->moveTo(x, y);
	}
}

void ControlPresenter::resize(float w, float h)
{
	PControl::resize(w, h);
	for (auto& child : children)
	{
		std::get<PControl*>(child)->resize(w, h);
	}
}

void ControlPresenter::draw(SDL_Renderer* ren)
{
	// Children are always drawn in ascending z-index order.
	for (auto& child : children)
	{
		std::get<PControl*>(child)->draw(ren);
	}

	PControl::draw(ren);
}

void ControlPresenter::notify(Event* e)
{
	// Children are always notified in descending z-index order.
	for (int i = children.size() - 1; i >= 0; --i)
	{
		std::get<EventSubscriber*>(children[i])->handleEvent(e);
	}
}

void ControlPresenter::handleEvent(Event* e)
{
	notify(e);
}

ControlPresenter::ControlPresenter(float x, float y, float w, float h) : PControl(x, y, w, h)
{
}

ControlPresenter::~ControlPresenter()
{
}

std::vector<PControl*> ControlPresenter::getChildren()
{
	std::vector<PControl*> result;

	for (auto& tuple : children)
	{
		result.push_back(std::get<PControl*>(tuple));
	}

	return result;
}
