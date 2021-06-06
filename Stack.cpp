#include "Stack.hpp"

void Stack::Setup(StackTypeEnum stackType, int x, int y)
{
	this->stackType = stackType;
	this->x = x;
	this->y = y;

	setAppropriateStackEnd();
}

void Stack::Render(ArtTools* artTools)
{
	if (cards.size() == 1 || stackType != StackTypeEnum::BottomStack)
		renderOnlyTopCard(artTools);
	else
		renderSpreadStack(artTools);
}

void Stack::renderOnlyTopCard(ArtTools* artTools)
{
	cards[cards.size() - 1]->Render(x, y, artTools);
}

void Stack::renderSpreadStack(ArtTools* artTools)
{
	size_t indexOfLastCard = cards.size() - 1;
	int offset = 0;

	if (indexOfLastCard > 1 && !cards[1]->FaceUp)
		cards[1]->Render(x, y - MINI_STEP, artTools);

	for (int c = 1; c < indexOfLastCard; c++)
		if (cards[c]->FaceUp)
			cards[c]->Render(x, y + offset++ * STEP_SIZE, artTools);

	cards[indexOfLastCard]->Render(x, y + offset * STEP_SIZE, artTools);
}

void Stack::AddCard(Card *card)
{
	cards.push_back(card);
}

void Stack::FlipTopCard()
{
	size_t indexOfLastCard = cards.size() - 1;

	cards[indexOfLastCard]->FaceUp = !cards[indexOfLastCard]->FaceUp;
}

void Stack::GetFreshDeck()
{
	for (int suitNumber = (int)CardSuit::Lowest; suitNumber <= (int)CardSuit::Highest; suitNumber++)
		for (int rankNumber = (int)CardRank::Lowest; rankNumber <= (int)CardRank::Highest; rankNumber++)
			cards.push_back(new Card((CardRank)rankNumber, (CardSuit)suitNumber));
}

void Stack::setAppropriateStackEnd()
{
	if (stackType == StackTypeEnum::TopStack)
		cards.push_back(Card::CreateAceAcceptingSlot());
	else if (stackType == StackTypeEnum::BottomStack)
		cards.push_back(Card::CreateKingAcceptingSlot());
	else
		cards.push_back(Card::CreateDeckBlankSlot());
}

bool Stack::IsEmpty()
{
	return cards.size() == 1;
}

void Stack::Shuffle()
{
	for (int c = SIZE_OF_DECK; c > 0; c--) // ignore the placeholder on the bottom, do grab the last card before to force face-down
	{
		int cardToSwap = blit::random() % c + 1;
		Card* card = cards[cardToSwap];
		cards[cardToSwap] = cards[c];
		card->FaceUp = false;
		cards[c] = card;
	}
}

Card* Stack::DrawCard()
{
	Card* card = cards[cards.size() - 1];

	cards.pop_back();

	return card;
}

int Stack::CountOfSelectableCards()
{
	int count = 0;

	for (int c = cards.size() - 1; c > 0; c--)
		if (cards[c]->FaceUp)
			count++;

	return count;
}

void Stack::RefillFromOtherStack(Stack* otherStack)
{
	while (!otherStack->IsEmpty())
		cards.push_back(otherStack->DrawCard()->Flip());
}

Card* Stack::GetCardForMove(int y, bool source)
{
	int count = cards.size(), faceUp = 0;

	if (HasTopCardNeedingFlipping())
		return NULL;

	if (y == TOP_ROW || (y == 1 && count == 1) || source == DESTINATION)
		return cards[count - 1];

	for (int c = 1; c < count; c++)
		if (cards[c]->FaceUp && ++faceUp == y)
			return cards[c];

	return NULL;
}

bool Stack::HasTopCardNeedingFlipping()
{
	if (IsEmpty())
		return false;

	return !cards[cards.size() - 1]->FaceUp;
}

bool Stack::IsTopCard(Card* card)
{
	return card == cards[cards.size() - 1];
}

Card* Stack::PeekTopCard()
{
	return cards[cards.size() - 1];
}