#pragma once

#include <SDL.h>
#include "../io/EventSubscriber.h"

class PControl : public EventSubscriber, public EventObserver
{
	public:
		virtual ~PControl();

		virtual void setX(float x);
		virtual void setY(float y);
		virtual void setWidth(float w);
		virtual void setHeight(float h);
		void setMinWidth(float minWidth);
		void setMinHeight(float minHeight);
		void setMaxWidth(float maxWidth);
		void setMaxHeight(float maxHeight);

		float getX() const;
		float getY() const;
		float getWidth() const;
		float getHeight() const;
		float getMinWidth() const;
		float getMinHeight() const;
		float getMaxWidth() const;
		float getMaxHeight() const;

		int getRoundedX() const;
		int getRoundedY() const;
		int getRoundedWidth() const;
		int getRoundedHeight() const;

		bool mouseInside(int x, int y) const;

		virtual void moveTo(float x, float y);
		virtual void resize(float w, float h);

		/*
		 *  Derived classes must call PControl's provided implementation
		 *  when implementing draw() to enable color and alpha effects.
		 */
		virtual void draw(SDL_Renderer* ren) = 0;

		std::vector<PControl*> getChildren() const;

		void notify(Event* e) override;
		void handleEvent(Event* e) override;

	protected:
		PControl(PControl* parent, float x, float y, float w, float h);
		void addChild(PControl* child);
		void removeChild(PControl* child);

	private:
		PControl* _parent;
		std::vector<PControl*> _children;

		float x;
		float y;
		float w;
		float h;
		float minW;
		float minH;
		float maxW;
		float maxH;
};