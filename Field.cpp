#include "Field.hpp"
#include "assets.hpp"

void Field::Setup()
{
	setupSDLGraphicsSystem();

	setupSlots();

	deal();
}

void Field::setupSlots()
{
	setupDrawAndDiscardDecks();

	setupUpperRow();

	setupLowerRow();
}

void Field::setupDrawAndDiscardDecks()
{
	DrawDeck.Setup(StackTypeEnum::DrawDeck, FIRST_COLUMN_X, TOP_ROW_Y);

	DrawDeck.GetFreshDeck();

	DrawDeck.Shuffle();

	DiscardDeck.Setup(StackTypeEnum::DiscardDeck, DISTANCE_BETWEEN_COLUMNS + FIRST_COLUMN_X, TOP_ROW_Y);
}

void Field::setupUpperRow()
{
	for (int slotNumber = LEFTMOST_COLUMN; slotNumber < UPPER_SLOTS; slotNumber++)
		UpperSlots[slotNumber].Setup(StackTypeEnum::TopStack, (slotNumber + ACE_SECTION_OFFSET) * DISTANCE_BETWEEN_COLUMNS + FIRST_COLUMN_X, TOP_ROW_Y);
}

void Field::setupLowerRow()
{
	for (int slotNumber = LEFTMOST_COLUMN; slotNumber < LOWER_SLOTS; slotNumber++)
		LowerSlots[slotNumber].Setup(StackTypeEnum::BottomStack, slotNumber * DISTANCE_BETWEEN_COLUMNS + FIRST_COLUMN_X, BOTTOM_ROW_Y);
}

void Field::deal()
{
	for (int slotNumber = LEFTMOST_COLUMN; slotNumber < LOWER_SLOTS; slotNumber++)
	{
		for (int cardNumber = 0; cardNumber < slotNumber; cardNumber++)
			LowerSlots[slotNumber].AddCard(DrawDeck.DrawCard());

		LowerSlots[slotNumber].AddCard(DrawDeck.DrawCard()->Flip());
	}
}

void Field::Render()
{
	artTools.CLS(errorCounter > 0 ? artTools.RED : artTools.GREEN);

	handleVibration();

	renderModeSpecificGraphics();
}

void Field::renderModeSpecificGraphics()
{
	if (inWinState)
		renderWinMessage();
	else if (waitingForResetConfirmation)
		renderResetConfirmation();
	else
		renderNormalMode();
}

void Field::renderNormalMode()
{
	drawStacks();

	drawCursors();
}

void Field::handleVibration()
{
	if (errorCounter > 0 && --errorCounter == 0)
		blit::vibration = VIBRATION_OFF;
}

void Field::drawStacks()
{
	DrawDeck.Render(&artTools);

	DiscardDeck.Render(&artTools);

	for (int slotNumber = LEFTMOST_COLUMN; slotNumber < UPPER_SLOTS; slotNumber++)
		UpperSlots[slotNumber].Render(&artTools);

	for (int slotNumber = LEFTMOST_COLUMN; slotNumber < LOWER_SLOTS; slotNumber++)
		LowerSlots[slotNumber].Render(&artTools);
}

void Field::drawCursors()
{
	const bool PRIMARY = true, SECONDARY = false;

	drawCursor(primaryCursorX, primaryCursorY, PRIMARY);

	if (secondaryCursorX != DISABLED)
		drawCursor(secondaryCursorX, secondaryCursorY, SECONDARY);
}

void Field::drawCursor(int x, int y, bool primary)
{
	blit::Point coordinates = getFieldCoordinatesFromCardCoordinates(x, y);

	coordinates.y += (primary ? (-CURSOR_PADDING - CURSOR_SIZE) : CURSOR_PADDING);
	blit::screen.sprite(primary ? PRIMARY_CURSOR : SECONDARY_CURSOR, coordinates);
}

blit::Point Field::getFieldCoordinatesFromCardCoordinates(int x, int y)
{
	int translatedX = x * DISTANCE_BETWEEN_COLUMNS + FIRST_COLUMN_X + Card::CARD_WIDTH + CURSOR_PADDING;
	int translatedY = (y == TOP_ROW) ? TOP_ROW_Y : (BOTTOM_ROW_Y + Stack::STEP_SIZE * (y - 1));

	return blit::Point(translatedX, translatedY);
}

void Field::fixYFollowingSidewaysMove()
{
	if (*activeCursorY == TOP_ROW)
		return;

	int maxY = max(LowerSlots[*activeCursorX].CountOfSelectableCards(), 1);

	if (*activeCursorY > maxY)
		*activeCursorY = maxY;
}

int Field::max(int x, int y)
{
	return (x > y) ? x : y;
}

void Field::setupPointersToActiveCursor()
{
	if (secondaryCursorX == DISABLED)
	{
		activeCursorX = &primaryCursorX;
		activeCursorY = &primaryCursorY;
	}
	else
	{
		activeCursorX = &secondaryCursorX;
		activeCursorY = &secondaryCursorY;
	}
}

void Field::moveCursorLeft()
{
	setupPointersToActiveCursor();

	if (--(*activeCursorX) < LEFTMOST_COLUMN)
		*activeCursorX = RIGHTMOST_COLUMN;

	if (*activeCursorY == TOP_ROW && *activeCursorX == UNUSED_SPACE)
		*activeCursorX = UNUSED_SPACE - 1;

	fixYFollowingSidewaysMove();
}

void Field::moveCursorRight()
{
	setupPointersToActiveCursor();

	if (++(*activeCursorX) > RIGHTMOST_COLUMN)
		*activeCursorX = LEFTMOST_COLUMN;

	if (*activeCursorY == TOP_ROW && *activeCursorX == UNUSED_SPACE)
		*activeCursorX = UNUSED_SPACE + 1;

	fixYFollowingSidewaysMove();
}

void Field::moveCursorUp()
{
	setupPointersToActiveCursor();

	if (--(*activeCursorY) < TOP_ROW || (*activeCursorY == TOP_ROW && *activeCursorX == UNUSED_SPACE))
	{
		*activeCursorY = LowerSlots[*activeCursorX].CountOfSelectableCards();
		if (*activeCursorY == 0)
			*activeCursorY = 1;
	}
}

void Field::moveCursorDown()
{
	setupPointersToActiveCursor();

	if (++(*activeCursorY) > LowerSlots[*activeCursorX].CountOfSelectableCards() && *activeCursorY > 1)
		*activeCursorY = TOP_ROW;

	if (*activeCursorY == TOP_ROW && *activeCursorX == UNUSED_SPACE)
		*activeCursorY = BOTTOM_ROW;
}

void Field::Update()
{
	for (ButtonsEnum button : buttonEventCreator.GenerateEvents(blit::buttons))
		handleEvent(button);

	inWinState = checkIfInWinState();
}

void Field::handleEvent(ButtonsEnum button)
{
	if (button == ButtonsEnum::B)
		cancel();
	else if (button == ButtonsEnum::Menu)
		resetRequest();
	else if (waitingForResetConfirmation || inWinState)
		return;
	else if (button == ButtonsEnum::Up)
		moveCursorUp();
	else if (button == ButtonsEnum::Down)
		moveCursorDown();
	else if (button == ButtonsEnum::Left)
		moveCursorLeft();
	else if (button == ButtonsEnum::Right)
		moveCursorRight();
	else if (button == ButtonsEnum::A)
		performAction();
	else if (button == ButtonsEnum::X)
		flipCardsAsNeeded();
	else if (button == ButtonsEnum::Y)
		setCursorToTopLeft();
}

void Field::resetRequest()
{
	if (inWinState || waitingForResetConfirmation)
		reset();
	else
		waitingForResetConfirmation = true;
}

void Field::cancel()
{
	unselect();
	waitingForResetConfirmation = false;
}

void Field::reset()
{
	recoverCards();

	waitingForResetConfirmation = false;

	DrawDeck.Shuffle();

	deal();
}

void Field::recoverCards()
{
	DrawDeck.RefillFromOtherStack(&DiscardDeck);

	for (int c = LEFTMOST_COLUMN; c < UPPER_SLOTS; c++)
		DrawDeck.RefillFromOtherStack(&UpperSlots[c]);

	for (int c = LEFTMOST_COLUMN; c < LOWER_SLOTS; c++)
		DrawDeck.RefillFromOtherStack(&LowerSlots[c]);
}

void Field::setupSDLGraphicsSystem()
{
	blit::set_screen_mode(blit::ScreenMode::hires);
	blit::screen.alpha = 255;
	blit::screen.mask = nullptr;
	blit::screen.sprites = blit::Surface::load(asset_cards_image);
}

void Field::performAction()
{
	if (primaryCursorX == secondaryCursorX && primaryCursorY == secondaryCursorY)
		unselect();
	else if (secondaryCursorX != DISABLED)
		setupAndCallMoveHandling();
	else if (primaryCursorX == LEFTMOST_COLUMN && primaryCursorY == TOP_ROW)
		drawDeckAction();
	else if (primaryCursorY == TOP_ROW && (primaryCursorX > UNUSED_SPACE || (primaryCursorX == DISCARD_DECK_COLUMN && DiscardDeck.IsEmpty())))
		noteError();
	else
		takeAppropriateActionForNormalStack();
}

void Field::takeAppropriateActionForNormalStack()
{
	Stack* stack = getStack(primaryCursorX, primaryCursorY, SOURCE);

	if (stack->HasTopCardNeedingFlipping())
		stack->FlipTopCard();
	else
		select();
}

void Field::select()
{
	secondaryCursorX = primaryCursorX;
	secondaryCursorY = primaryCursorY;
}

void Field::unselect()
{
	secondaryCursorX = DISABLED;
}

void Field::setupAndCallMoveHandling()
{
	Stack* sourceStack = getStack(primaryCursorX, primaryCursorY, SOURCE);
	Stack* destinationStack = getStack(secondaryCursorX, secondaryCursorY, DESTINATION);
	Card* sourceCard = (sourceStack == NULL) ? NULL : sourceStack->GetCardForMove(primaryCursorY, SOURCE);
	Card* destinationCard = (destinationStack == NULL) ? NULL : destinationStack->GetCardForMove(secondaryCursorY, DESTINATION);

	postMoveHandling(callAppropriateMoveFunctionsReturningError(sourceCard, destinationCard, sourceStack, destinationStack));
}

bool Field::callAppropriateMoveFunctionsReturningError(Card* sourceCard, Card* destinationCard, Stack* sourceStack, Stack* destinationStack)
{
	if (sourceCard == NULL || destinationCard == NULL)
		return true;
	else if (secondaryCursorY == TOP_ROW && sourceCard->CanSitOnInUpperLevel(destinationCard) && sourceStack->IsTopCard(sourceCard))
		moveOneCard(sourceStack, destinationStack);
	else if (secondaryCursorY > TOP_ROW && sourceCard->CanSitOnInLowerLevel(destinationCard))
		moveMultipleCards(sourceCard, sourceStack, destinationStack);
	else
		return true;

	return false;
}

void Field::postMoveHandling(bool errorFound)
{
	if (errorFound)
		noteError();
	else
		advanceCursor();
}

void Field::moveOneCard(Stack* sourceStack, Stack* destinationStack)
{
	destinationStack->AddCard(sourceStack->DrawCard());
}

void Field::moveMultipleCards(Card* sourceCard, Stack* sourceStack, Stack* destinationStack)
{
	std::vector<Card*> tempStack;

	do
		tempStack.push_back(sourceStack->DrawCard());
	while (tempStack[tempStack.size() - 1] != sourceCard);

	do
	{
		destinationStack->AddCard(tempStack[tempStack.size() - 1]);
		tempStack.pop_back();
	} while (tempStack.size() > 0);
}

void Field::advanceCursor()
{
	primaryCursorX = secondaryCursorX;
	primaryCursorY = secondaryCursorY;

	secondaryCursorX = DISABLED;
}

void Field::drawDeckAction()
{
	if (DrawDeck.IsEmpty())
		DrawDeck.RefillFromOtherStack(&DiscardDeck);
	else
		turnOverCardFromDrawDeck();
}

void Field::turnOverCardFromDrawDeck()
{
	DiscardDeck.AddCard(DrawDeck.DrawCard()->Flip());
}

void Field::noteError()
{
	errorCounter = CYCLES_PER_ERROR;
	blit::vibration = VIBRATION_ON;
}

Stack* Field::getStack(int x, int y, bool source)
{
	if (y != TOP_ROW)
		return &LowerSlots[x];
	
	if (x == LEFTMOST_COLUMN)
		return NULL;

	if (x == DISCARD_DECK_COLUMN)
		return source ? &DiscardDeck : NULL;

	return &UpperSlots[x - 3];
}

Card* Field::getCard(int x, int y, bool source)
{
	Stack* stack = getStack(x, y, source);

	if (stack == NULL)
		return NULL;

	return stack->GetCardForMove(y, source);
}

bool Field::checkIfInWinState()
{
	for (int c = 0; c < UPPER_SLOTS; c++)
		if (UpperSlots[c].PeekTopCard()->Rank != CardRank::K)
			return false;

	return true;
}

void Field::renderWinMessage()
{
	artTools.DrawCenteredText("YOU WON!", artTools.BLACK);
}

void Field::renderResetConfirmation()
{
	artTools.DrawCenteredText("ARE YOU SURE YOU WANT TO RESET?\nIF SO, PRESS RESET (MENU) AGAIN.\nIF NOT, PRESS CANCEL (B).", artTools.RED);
}

void Field::flipCardsAsNeeded()
{
	for (int c = LEFTMOST_COLUMN; c <= RIGHTMOST_COLUMN; c++)
		if (LowerSlots[c].HasTopCardNeedingFlipping())
			LowerSlots[c].FlipTopCard();
}

void Field::setCursorToTopLeft()
{
	primaryCursorX = 0;
	primaryCursorY = 0;
	secondaryCursorX = DISABLED;
}