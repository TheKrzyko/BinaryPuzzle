#include "Board.h"
#include "conio2.h"
#include <stdlib.h>


Board::Board()
{
	
}

Board::~Board()
{
	if (boardArray != nullptr)
		deleteBoardArray();
}

void Board::setBoardSize(const int size)
{
	if (size % 2 == 1)
		return;
	if (boardArray != nullptr)
	{
		deleteBoardArray();
	}
	boardSize = size;
	createBoardArray(boardSize);
}

void Board::setNewGame()
{
	setBoardSize(defaultSize);
	for(int i = 0; i < defaultSize; i++)
	{
		for(int j = 0; j < defaultSize; j++)
		{
			boardArray[i][j] = defaultArray[i][j];
		}
	}
}

void Board::setRandomBoard()
{
	for (int y = 0; y < boardSize; y++)
	{
		for (int x = 0; x < boardSize; x++)
		{
			boardArray[y][x] = emptyChar;
		}
	}

	int maxError = (boardSize*boardSize*maxPctError)/100;
	int maxFields = ((boardSize*boardSize)*(rand() % (maxPctFill - minPctFill) + minPctFill)) / 100;
	int currentFields = 0;
	int currentErrors = 0;
	while(currentErrors < maxError && currentFields < maxFields)
	{
		int x = rand() % boardSize;
		int y = rand() % boardSize;
		int value = rand() % 2 + 2;//value wont be modificable on board
		if(trySetField(x, y, value))
		{
			currentFields++;
		}
		else
		{
			currentErrors++;
		}
	}
}

int Board::getSize()
{
	return boardSize;
}

char Board::getEmptyChar()
{
	return emptyChar;
}

Field Board::getField(int x, int y)
{
	char field = boardArray[y][x];
	if (field < '0' || field > '3')
		return { true, false, empty };
	else
		return { false, (bool)((field - '0') / 2), (field - '0') % 2 };
}

void Board::setField(int x, int y, Field field)
{
	if (field.isEmpty)
		boardArray[y][x] = emptyChar;
	else if (field.isLocked)
		boardArray[y][x] = field.value + 2;
	else
		boardArray[y][x] = '0' + field.value;
}

bool Board::trySetField(int x, int y, int value)
{
	if(canSetField(x, y, value))
	{
		setField(x, y, {false, false, value});
		return true;
	}
	return false;
}

bool Board::canSetField(int x, int y, int value)
{
	Field field = getField(x, y);
	if (field.isEmpty || !field.isLocked)
	{
		if (!isValidRule1(x, y, value) ||
			!isValidRule2(x, y, value) ||
			!isValidRule3(x, y, value))
			return false;
		else
			return true;
	}
	return false;
}

void Board::print(const int x, const int y)
{
	gotoxy(x, y);
	for(int i = 0; i < boardSize; i++)
	{
		gotoxy(x, y + i);
		for(int j = 0; j < boardSize; j++)
		{
			int a = boardArray[i][j] - '0';
			if(a < 0 || a > 3)
			{
				putch(emptyChar);
			}
			else if(a/2 == 1)
			{
				textbackground(backColor);
				textcolor(frontColor);
				putch((a%2)+'0');
				textbackground(BLACK);
				textcolor(frontColor);
			}
			else
			{
				textcolor(LIGHTGRAY);
				putch(boardArray[i][j]);
			}
		}
	}
	textcolor(LIGHTGRAY);
}

bool Board::save(FILE* file)
{
	fprintf(file, "%d\n", boardSize);
	for(int y = 0; y < boardSize; y++)
	{
		for(int x = 0; x < boardSize; x++)
		{
			fputc(boardArray[y][x], file);
			
		}
	}
	return true;
}

bool Board::load(FILE* file)
{
	int newBoardSize;
	fscanf(file, "%d\n", &newBoardSize);
	setBoardSize(newBoardSize);
	for(int y = 0; y < boardSize; y++)
	{
		for(int x = 0; x < boardSize; x++)
		{
			boardArray[y][x] = fgetc(file);
		}
	}
	return true;
}

void Board::createBoardArray(const int size)
{
	
	boardSize = size;
	boardArray = new char*[size];
	for (int i = 0; i < size; i++)
		boardArray[i] = new char[size];
	for(int y = 0; y < size; y++)
	{
		for(int x = 0; x < size; x++)
		{
			boardArray[y][x] = emptyChar;
		}
	}
}

void Board::deleteBoardArray()
{
	for (int i = 0; i < boardSize; i++)
	{
		delete[] boardArray[i];
	}
	delete[] boardArray;
}

int Board::valueOf(int x, int y)
{
	return (boardArray[y][x] >= '0' && boardArray[y][x] <= '3' ? (boardArray[y][x]-'0')%2 : empty);
}

int Board::countInRow(int row, int value)
{
	int result = 0;
	for(int i = 0; i < boardSize; i++)
	{
		if (valueOf(i, row) == value)
			result++;
	}
	return result;
}

int Board::countInCol(int col, int value)
{
	int result = 0;
	for (int i = 0; i < boardSize; i++)
	{
		if (valueOf(col, i) == value)
			result++;
	}
	return result;
}

bool Board::isRowFull(int y)
{
	for(int i = 0; i < boardSize; i++)
	{
		if(valueOf(i, y) == empty)
			return false;;
	}
	return true;
}

bool Board::isColFull(int x)
{
	for (int i = 0; i < boardSize; i++)
	{
		if (valueOf(x, i) == empty)
			return false;;
	}
	return true;
}

bool Board::loadBoardFromFile(FILE* file)
{
	char field;
	for(int y = 0; y < boardSize; y++)
	{
		for(int x = 0; x <= boardSize; x++)
		{
			if (x == boardSize && y == boardSize - 1)
				break;
			field = fgetc(file);
			if (field == EOF)
				return false;
			if(field != '\n')
			{
				if (field == '0' || field == '1')
					boardArray[y][x] = field + 2;
				else
					boardArray[y][x] = field;
			}
		}
	}
	return true;
}

bool Board::isValidRule1(int x, int y, int value, char* reason, size_t reasonSize)
{
	
	bool result = true;
	char buffer = boardArray[y][x];
	boardArray[y][x] = '0' + value;
	int sameValueRow = 1;
	int sameValueCol = 1;
	for(int i = 1; i < boardSize; i++)
	{
		if (valueOf(i, y) == empty)
			sameValueRow = 1;
		else if (valueOf(i - 1, y) == valueOf(i, y))
			sameValueRow++;
		else
			sameValueRow = 1;

		if (valueOf(x, i) == empty)
			sameValueCol = 1;
		else if (valueOf(x, i - 1) == valueOf(x, i))
			sameValueCol++;
		else
			sameValueCol = 1;

		if(sameValueRow == 3 || sameValueCol == 3)
		{
			if(reason != nullptr)
			{
				sprintf_s(reason, reasonSize, "%s%d%s", "Po wpisaniu ", value, " pojawia sie 3 identyczne liczby obok siebie.");
			}
			result = false;
			break;
		}
	}
	boardArray[y][x] = buffer;
	
	return result;
}

bool Board::isValidRule2(int x, int y, int value, char* reason, size_t reasonSize)
{
	bool result = true;
	char buffer = boardArray[y][x];
	boardArray[y][x] = '0' + value;

	if (countInCol(x, value) > boardSize / 2)
	{
		result = false;
		if (reason != nullptr)
		{
			sprintf_s(reason, reasonSize, "%s%d%s%d%s", "Po wpisaniu ", value, " bedzie za duzo ", value, " w kolumnie.");
		}
	}
	if (countInRow(y, value) > boardSize / 2)
	{
		result = false;
		if (reason != nullptr)
		{
			sprintf_s(reason, reasonSize, "%s%d%s%d%s", "Po wpisaniu ", value, " bedzie za duzo ", value, " w wierszu.");
		}
	}

	boardArray[y][x] = buffer;
	return result;
}

bool Board::isValidRule3(int x, int y, int value, char* reason, size_t reasonSize)
{
	bool result = true;
	char buffer = boardArray[y][x];
	boardArray[y][x] = '0' + value;
	
	if(isColFull(x) || isRowFull(y))
	{
		for(int i = 0; i < boardSize; i++)
		{
			if(i != x && isColFull(i))
			{
				bool same = true;
				for(int j = 0; j < boardSize; j++)
				{
					if (valueOf(i, j) != valueOf(x, j))
						same = false;
				}
				if(same)
				{
					result = false;
					if (reason != nullptr)
					{
						sprintf_s(reason, reasonSize, "%s%d%s%d%s", "Po wpisaniu ", value, " kolumna bedzie identyczny z kolumna nr ", i,".");
					}					
					break;
				}
			}
			if(i != y && isRowFull(i))
			{
				bool same = true;
				for (int j = 0; j < boardSize; j++)
				{
					if (valueOf(j, i) != valueOf(j, y))
						same = false;
				}
				if (same)
				{
					result = false;
					if (reason != nullptr)
					{
						sprintf_s(reason, reasonSize, "%s%d%s%d%s", "Po wpisaniu ", value, " wiersz bedzie identyczny z wierszem  nr ", i, ".");
					}
					break;
				}
			}
		}
	}

	boardArray[y][x] = buffer;
	return result;
}
