#pragma once
#include "Field.h"
#include "conio2.h"
#include <stdio.h>

class Board
{
public:
	Board();
	~Board();

	/**
	 * \brief Function changes board size.
	 * \param size Size should be even and larger than 0.
	 */
	void setBoardSize(int size);
	
	/**
	 * \brief Sets default board.
	 */
	void setNewGame();
	
	/**
	 * \brief Fills board with compatible fields.
	 */
	void setRandomBoard();

	int getSize();
	
	/**
	 * \return Character that is used to fill empty fields.
	 */
	char getEmptyChar();

	Field getField(int x, int y);
	void setField(int x, int y, Field field);
	
	/**
	 * \brief Sets given position with value as long as its complies with rules.
	 * \return true if successful
	 */
	bool trySetField(int x, int y, int value);

	/**
	 * \return true if value can be placed on given position
	 */
	bool canSetField(int x, int y, int value);

	void print(const int x, const int y);
	
	bool save(FILE* file);
	bool load(FILE* file);
	bool loadBoardFromFile(FILE* file);

	bool isValidRule1(int x, int y, int value, char* reason = nullptr, size_t reasonSize = 0);
	bool isValidRule2(int x, int y, int value, char* reason = nullptr, size_t reasonSize = 0);
	bool isValidRule3(int x, int y, int value, char* resaon = nullptr, size_t reasonSize = 0);

	int countInRow(int row, int value);
	int countInCol(int col, int value);
private:
	const int minPctFill = 10;
	const int maxPctFill = 50;
	const int maxPctError = 30;

	const char emptyChar = ' ';
	
	/**
	 * \brief Background color of locked fields.
	 */
	const int backColor = BLUE;
	/**
	 * \brief Text color of locked fields.
	 */
	const int frontColor = LIGHTCYAN;

	const int empty = -1;
	const int defaultSize = 12;
	const char defaultArray[12][12] = {
	{ ' ',' ',' ','3',' ',' ',' ',' ',' ',' ',' ','3'},
	{ ' ','2',' ',' ',' ',' ',' ',' ','3',' ',' ',' '},
	{ ' ',' ',' ',' ',' ','2',' ',' ',' ',' ',' ',' '},
	{ ' ','2',' ','3',' ','2',' ','2',' ','2',' ','2'},
	{ '3',' ','3',' ',' ',' ',' ','2',' ',' ','3',' '},
	{ ' ',' ',' ',' ',' ','2',' ',' ','3',' ',' ',' '},
	{ ' ',' ','3','3',' ',' ',' ',' ','3',' ',' ',' '},
	{ '2',' ',' ',' ',' ',' ','2',' ',' ',' ',' ','2'},
	{ ' ','3',' ',' ','3',' ',' ',' ',' ',' ','3',' '},
	{ ' ',' ','2',' ',' ','2',' ','2',' ',' ',' ',' '},
	{ ' ',' ','2',' ',' ','2','2',' ',' ',' ',' ','2'},
	{ ' ',' ',' ','3',' ',' ','2',' ',' ','3',' ',' '}
	};

	void createBoardArray(const int size);
	void deleteBoardArray();


	/**
	 * \return Value on given position.
	 * \return -1 if field is empty.
	 */
	int valueOf(int x, int y);
	bool isRowFull(int y);
	bool isColFull(int x);


	int boardSize;
	char** boardArray = nullptr;
};

