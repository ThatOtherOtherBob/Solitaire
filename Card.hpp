#pragma once
#include "CardRank.hpp"
#include "CardSuit.hpp"
#include "ArtTools.hpp"

class Card
{
public:
	Card(CardRank rank, CardSuit suit);
	CardRank Rank;
	CardSuit Suit;
	bool FaceUp = false;
	void Render(int x, int y, ArtTools* artTools);
	bool CanSitOnInUpperLevel(Card* cardToSitOn);
	bool CanSitOnInLowerLevel(Card* cardToSitOn);
	Card* Flip();
	static Card* CreateAceAcceptingSlot();
	static Card* CreateKingAcceptingSlot();
	static Card* CreateDeckBlankSlot();
	static const int CARD_WIDTH = 30, CARD_HEIGHT = 50;
private:
	const int RANK_X_OFFSET = 3, RANK_Y_OFFSET = 3, SUIT_X_OFFSET = 13, SUIT_Y_OFFSET = 3;
	bool isSlot();
	void renderFront(int x, int y, ArtTools* artTools);
	void renderBack(int x, int y, ArtTools* artTools);
	void renderSpace(int x, int y, ArtTools* artTools);
	bool isRed();
	int getRankImage();
	int getSuitImage();
};