#include "PUI.h"
#include <iostream>

using namespace std;

PUI::PUI(SDL_Window* win, SDL_Renderer* ren, int ww, int hh) : win(win), ren(ren), ww(ww), wh(hh), test((int)(ww / 6.0), (int)(wh / 3.0), (int)((2 * ww) / 3.0), (int)((2 * wh) / 5.0), 4, 2)
{
	std::string mplusPath = utils::getcwd() + "\\assets\\mplus-2p-light.ttf";

	cout << mplusPath << endl;

	mplus = TTF_OpenFont(mplusPath.c_str(), 64);

	TTF_SetFontHinting(mplus, TTF_HINTING_LIGHT);

	Label* testLabel = new Label(mplus, 0, 0, 0, 0);
	test[0][0].setElement(testLabel);

	testLabel->setText("hpqd11test22lkofsnaso", ren);
	testLabel->setClipState(LabelClippingState::PAN);

	TTF_CloseFont(mplus);
}

void PUI::draw()
{
	test.draw(ren);
}