#pragma once
#include<vector>
#include "Card.hpp"
#include "StackTypeEnum.hpp"

struct Stack
{
public:
	void Setup(StackTypeEnum stackType, int x, int y);
	void Render(ArtTools* artTools);
	void AddCard(Card *card);
	void FlipTopCard();
	bool IsEmpty();
	Card* DrawCard();
	void Shuffle();
	int CountOfSelectableCards();
	static const int STEP_SIZE = 12, MINI_STEP = 3;
	void RefillFromOtherStack(Stack* otherStack);
	Card* GetCardForMove(int y, bool source);
	bool HasTopCardNeedingFlipping();
	bool IsTopCard(Card* card);
	Card* PeekTopCard();
	void GetFreshDeck();
private:
	const int SIZE_OF_DECK = 52, TOP_ROW = 0;
	const bool SOURCE = true, DESTINATION = false;
	int x, y;
	StackTypeEnum stackType;
	std::vector<Card*> cards;
	void setAppropriateStackEnd();
	void renderOnlyTopCard(ArtTools* artTools);
	void renderSpreadStack(ArtTools* artTools);
};
