#pragma once
#include "Board.h"
#include "Input.h"
#include "List.h"

class Game
{
public:
	Game(char* title);
	~Game();
	bool isRunning() const;

	void update();
	void render();

private:
	const int infoX = 2;//2,2
	const int infoY = 2;
	const int infoWidth = 66;
	const int infoHeight = 28;
	const int inputX = infoX;
	const int inputY = infoY + infoHeight - 1;
	const int outputX = infoX;
	const int outputY = infoY + 19;
	const int boardX = 76;//76, 4
	const int boardY = 4;

	int cursorX = 0;
	int cursorY = 0;
	char textBuffor[200];

	Board board;
	Input input;

	bool running = true;
	//--These turns to false when rendered--//
	bool redrawView = true;					//
	bool redrawInfo = false;				//
	bool redrawBoard = false;				//
	//--------------------------------------//

	bool impossibleFieldsShown = false;
	bool drawSides = false;
	bool autoDetectEnd = false;
	bool evidentFieldsShown = false;

	bool loadGame(FILE* file);
	bool saveGame(FILE* file);
	
	enum Direction { RightDir, DownDir };
	void printInfo(int x, int y);
	void printBorder(const int x, const int y, const int width, const int height);
	void printLine(const int x, const int y, const int length, const Direction dir);
	void printSide(int x, int y);

	void clearArea(int x, int y, int width, int height);
	void clearInputArea();
	void clearOutputArea();


	void showHint();
	bool showImpossibleFields();

	struct evidentField { int x; int y; int value; };
	List<evidentField> evidentFields;
	/**
	 * \brief Prints evident fields on screen, and adds these fields to evidentFields list.
	 */
	void showEvidentFields();
	/**
	 * \brief Fills board with evidentFields list.
	 */
	void setEvidentFields();

	void handleInput();
	void setCursorOnBoard();
};
