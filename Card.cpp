#include "Card.hpp"
#include "game.hpp"

Card::Card(CardRank rank, CardSuit suit)
{
	Rank = rank;
	Suit = suit;
}

int Card::getRankImage()
{
	const int OFFSET_FOR_RED_ROW = -1, OFFSET_FOR_BLACK_ROW = 15, BLANK = 13;

	if (Rank == CardRank::AceAcceptingSlot || Rank == CardRank::KingAcceptingSlot)
		return BLANK;

	return (int)Rank + (isRed() ? OFFSET_FOR_RED_ROW : OFFSET_FOR_BLACK_ROW);
}

int Card::getSuitImage()
{
	const int OFFSET_FOR_SUIT = 32, BLANK = 13;

	if (Rank == CardRank::AceAcceptingSlot || Rank == CardRank::KingAcceptingSlot)
		return BLANK;

	return (int)Suit + OFFSET_FOR_SUIT;
}

bool Card::isRed()
{
	return Suit == CardSuit::Heart || Suit == CardSuit::Diamond;
}

bool Card::CanSitOnInUpperLevel(Card* cardToSitOn)
{
	if (!FaceUp)
		return false;

	return ((int)cardToSitOn->Rank + 1 == (int)Rank && cardToSitOn->Suit == Suit && cardToSitOn->FaceUp)
		||
		(cardToSitOn->Rank == CardRank::AceAcceptingSlot && Rank == CardRank::A);
}

bool Card::CanSitOnInLowerLevel(Card* cardToSitOn)
{
	if (!FaceUp)
		return false;

	return ((int)cardToSitOn->Rank - 1 == (int)Rank && cardToSitOn->isRed() != isRed() && cardToSitOn->FaceUp)
		||
		(cardToSitOn->Rank == CardRank::KingAcceptingSlot && Rank == CardRank::K);
}

Card* Card::Flip()
{
	FaceUp = !FaceUp;

	return this;
}

Card* Card::CreateAceAcceptingSlot()
{
	return new Card(CardRank::AceAcceptingSlot, CardSuit::Lowest);
}

Card* Card::CreateKingAcceptingSlot()
{
	return new Card(CardRank::KingAcceptingSlot, CardSuit::Lowest);
}

Card* Card::CreateDeckBlankSlot()
{
	return new Card(CardRank::DeckBlank, CardSuit::Lowest);
}

bool Card::isSlot()
{
	return Rank == CardRank::AceAcceptingSlot || Rank == CardRank::KingAcceptingSlot || Rank == CardRank::DeckBlank;
}

void Card::Render(int x, int y, ArtTools* artTools)
{
	if (isSlot())
		renderSpace(x, y, artTools);
	else if (FaceUp)
		renderFront(x, y, artTools);
	else
		renderBack(x, y, artTools);
}

void Card::renderFront(int x, int y, ArtTools* artTools)
{
	const int INSET = 1;

	artTools->NestedBoxes(x, y, CARD_WIDTH, CARD_HEIGHT, INSET, artTools->BLACK, artTools->WHITE);

	blit::screen.sprite(getRankImage(), blit::Point(x + RANK_X_OFFSET, y + RANK_Y_OFFSET));
	blit::screen.sprite(getSuitImage(), blit::Point(x + SUIT_X_OFFSET, y + SUIT_Y_OFFSET));
}

void Card::renderBack(int x, int y, ArtTools* artTools)  // todo: get rid of magic #s
{
	const int INSET = 3;

	artTools->NestedBoxes(x, y, CARD_WIDTH, CARD_HEIGHT, INSET, artTools->WHITE, artTools->RED);
}

void Card::renderSpace(int x, int y, ArtTools* artTools)  // todo: get rid of magic #s
{
	const int INSET = 1;

	artTools->NestedBoxes(x, y, CARD_WIDTH, CARD_HEIGHT, INSET, artTools->BLACK, artTools->GREEN);
}
