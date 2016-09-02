#pragma once

#include <SDL.h>
#include "PControl.h"

class PWindow : public PControl
{
	public:
		// PWindow(std::string windowName, PWindow* owner, int windowX, int windowY, int windowWidth, int windowHeight);
		static PWindow* createWindow(std::string windowName, PWindow* owner, int windowX, int windowY, int windowWidth, int windowHeight);
		~PWindow();

		PWindow* getOwner();

		void setX(float x) override;
		void setY(float y) override;
		void setWidth(float w) override;
		void setHeight(float h) override;
		void moveTo(float x, float y) override;
		void resize(float w, float h) override;

		void draw(SDL_Renderer* ren) override;
		void handleEvent(Event* e) override;

	private:
		const PWindow* _owner;
		PWindow(std::string windowName, PWindow* owner, int windowX, int windowY, int windowWidth, int windowHeight);

		const int _DRAG_ZONE_DIST = 5;
		SDL_Window* _window;
		SDL_Renderer* _renderer;

		void OnMouseDown(MouseDownEvent& e);
		void OnMouseUp(MouseUpEvent& e);
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
