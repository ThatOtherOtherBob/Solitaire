#pragma once
#include "32blit.hpp"

struct ArtTools
{
	const blit::Pen BLACK = blit::Pen(0, 0, 0);
	const blit::Pen GREEN = blit::Pen(0, 255, 0);
	const blit::Pen RED = blit::Pen(255, 0, 0);
	const blit::Pen WHITE = blit::Pen(255, 255, 255);
	const blit::Rect SCREEN_BOUNDS = blit::Rect(0, 0, 319, 239);

	void CLS(blit::Pen color);
	void Box(int x, int y, int width, int height, blit::Pen color);
	void NestedBoxes(int x, int y, int width, int height, int insetBy, blit::Pen outerColor, blit::Pen innerColor);
	void DrawCenteredText(std::string text, blit::Pen color);
};