#pragma once
#include "Stack.hpp"
#include "ButtonEventCreator.hpp"

class Field
{
private:
	ArtTools artTools;
	static const int UPPER_SLOTS = 4, LOWER_SLOTS = 7;
	static const int DRAW_DECK_X = 7, DRAW_DECK_Y = 10;
	static const int TOP_ROW_Y = 10, BOTTOM_ROW_Y = 70, DISTANCE_BETWEEN_COLUMNS = 45, FIRST_COLUMN_X = 7;
	static const int CURSOR_SIZE = 8;
	static const int PRIMARY_CURSOR = 36, SECONDARY_CURSOR = 37, CURSOR_PADDING = 3, DISABLED = -1;
	static const int LEFTMOST_COLUMN = 0, RIGHTMOST_COLUMN = 6, UNUSED_SPACE = 2, TOP_ROW = 0, BOTTOM_ROW = 1, DISCARD_DECK_COLUMN = 1;
	static const int ACE_SECTION_OFFSET = 3;
	static const int CYCLES_PER_ERROR = 10;
	static const bool SOURCE = true, DESTINATION = false;
	const float VIBRATION_ON = 1, VIBRATION_OFF = 0;
	int* activeCursorX;
	int* activeCursorY;
	int errorCounter = 0;
	int primaryCursorX = 0, primaryCursorY = 0, secondaryCursorX = -1, secondaryCursorY = -1;
	Stack DrawDeck;
	Stack DiscardDeck;
	Stack UpperSlots[Field::UPPER_SLOTS];
	Stack LowerSlots[Field::LOWER_SLOTS];
	bool waitingForResetConfirmation = false, inWinState = false;
	void setupPointersToActiveCursor();
	void setupDrawAndDiscardDecks();
	void setupUpperRow();
	void setupLowerRow();
	void deal();
	ButtonEventCreator buttonEventCreator;
	void moveCursorLeft();
	void moveCursorRight();
	void moveCursorUp();
	void moveCursorDown();
	void drawStacks();
	void drawCursors();
	void drawCursor(int x, int y, bool primary);
	blit::Point getFieldCoordinatesFromCardCoordinates(int x, int y);
	void setupSDLGraphicsSystem();
	void performAction();
	void select();
	void unselect();
	void drawDeckAction();
	void turnOverCardFromDrawDeck();
	void noteError();
	Stack* getStack(int x, int y, bool source);
	Card* getCard(int x, int y, bool source);
	void setupAndCallMoveHandling();
	bool callAppropriateMoveFunctionsReturningError(Card* sourceCard, Card* destinationCard, Stack* sourceStack, Stack* destinationStack);
	void moveOneCard(Stack* sourceStack, Stack* destinationStack);
	void moveMultipleCards(Card* sourceCard, Stack* sourceStack, Stack* destinationStack);
	void postMoveHandling(bool errorFound);
	void advanceCursor();
	bool checkIfInWinState();
	void renderWinMessage();
	void renderResetConfirmation();
	void renderNormalMode();
	void resetRequest();
	void reset();
	void cancel();
	void recoverCards();
	void handleEvent(ButtonsEnum button);
	void setupSlots();
	void handleVibration();
	void renderModeSpecificGraphics();
	void fixYFollowingSidewaysMove();
	void takeAppropriateActionForNormalStack();
	int max(int x, int y);
	void flipCardsAsNeeded();
	void setCursorToTopLeft();
public:
	void Setup();
	void Render();
	void Update();
};
