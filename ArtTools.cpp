#include "ArtTools.hpp"


void ArtTools::CLS(blit::Pen color)
{
	blit::screen.pen = color;

	blit::screen.clear();
}


void ArtTools::Box(int x, int y, int width, int height, blit::Pen color)
{
	blit::screen.pen = color;

	blit::screen.rectangle(blit::Rect(x, y, width, height));
}

void ArtTools::NestedBoxes(int x, int y, int width, int height, int insetBy, blit::Pen outerColor, blit::Pen innerColor)
{
	Box(x, y, width, height, outerColor);

	Box(x + insetBy, y + insetBy, width - insetBy * 2, height - insetBy * 2, innerColor);
}

void ArtTools::DrawCenteredText(std::string text, blit::Pen color)
{
	blit::screen.pen = color;
	blit::screen.text(text, blit::outline_font, SCREEN_BOUNDS, true, blit::center_center);
}
