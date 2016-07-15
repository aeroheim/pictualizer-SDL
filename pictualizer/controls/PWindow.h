#pragma once

#include <SDL.h>
#include "../controls/ControlPresenter.h"

class PWindow : public ControlPresenter
{
	public:
		PWindow(std::string windowName, int windowX, int windowY, int windowWidth, int windowHeight);
		~PWindow();

		void setX(float x) override;
		void setY(float y) override;
		void setWidth(float w) override;
		void setHeight(float h) override;
		void moveTo(float x, float y) override;
		void resize(float w, float h) override;

		void handleEvent(Event* e) override;

	private:
		const int DRAG_ZONE_DIST = 15;
		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Event e;

		void OnMouseButtonDown(MouseDownEvent& e);
		void OnMouseButtonUp(MouseUpEvent& e);
		void OnMouseMotion(MouseMotionEvent& e) const;
		void OnMouseDrag(MouseDragEvent& e);

		int _globalMouseDownX = 0;
		int _globalMouseDownY = 0;
		int _resizeStartWindowWidth = 0;
		int _resizeStartWindowHeight = 0;

		bool _dragResizing = false;

		bool mouseDownInResizeZone(int mouseX, int mouseY) const;
		void setResizeCursor(MouseMotionEvent& e) const;
		void dragResize();
};
