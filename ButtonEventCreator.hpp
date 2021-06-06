#pragma once
#include "32blit.hpp"
#include<vector>
#include "ButtonsEnum.hpp"

class ButtonEventCreator
{
public:
	ButtonEventCreator();
	std::vector<ButtonsEnum> GenerateEvents(blit::ButtonState buttons);
private:
	bool oldButtons[(int)ButtonsEnum::Size];
	bool newButtons[(int)ButtonsEnum::Size];
	void setOldButtons();
	void setNewButtons(blit::ButtonState buttons);
	std::vector<ButtonsEnum> createPressedEvents();
};