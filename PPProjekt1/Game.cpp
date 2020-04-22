#include "Game.h"
#include "conio2.h"
#include "Input.h"
#include <cstring>

#define getLength(arr) sizeof arr / sizeof arr[0]

Game::Game(char* title)
{
	settitle(title);
	running = true;
	input.add(Exit, 0x1b);
	input.add(NewGame, 'n', 'N');
	input.add(Up, 0, 0x48);
	input.add(Down, 0, 0x50);
	input.add(Right, 0, 0x4d);
	input.add(Left, 0, 0x4b);
	input.add(Zero, '0');
	input.add(One, '1');
	input.add(RandomFill, 'o', 'O');
	input.add(ChangeSize, 'r', 'R');
	input.add(SimpleHint, 'p', 'P');
	input.add(ShowImpossibleFields, 'k', 'K');
	input.add(ShowSides, 'd', 'D');
	input.add(AutoDetectEnd, 'a', 'A');
	input.add(EvidentFields, 'j', 'J');
	input.add(SetEvidentFields, 'w', 'W');
	input.add(SaveGame, 's', 'S');
	input.add(LoadGame, 'l', 'L');
	board.setNewGame();
	_setcursortype(_SOLIDCURSOR);
}

Game::~Game()
{
}

bool Game::isRunning() const
{
	return running;
}

void Game::update()
{
	handleInput();
}

void Game::render()
{
	if (redrawView || redrawInfo || redrawBoard)
	{
		_setcursortype(_NOCURSOR);

		if (redrawView)
		{
			clrscr();
			printBorder(infoX - 1, infoY - 1, infoWidth + 2, infoHeight + 2);
			printBorder(boardX - 1, boardY - 1, board.getSize() + 2, board.getSize() + 2);
		}
		if (redrawInfo || redrawView)
			printInfo(infoX, infoY);
		if (redrawBoard || redrawView)
		{
			board.print(boardX, boardY);
			if (drawSides)
			{
				gotoxy(boardX - 3, boardY - 3);
				cputs("1->");
				gotoxy(boardX - 3, boardY - 2);
				cputs("||");
				gotoxy(boardX - 3, boardY - 1);
				cputs("\\/");
				gotoxy(boardX + board.getSize(), boardY + board.getSize() + 2);
				cputs("<-0");
				gotoxy(boardX + board.getSize() + 1, boardY + board.getSize());
				cputs("/\\");
				gotoxy(boardX + board.getSize() + 1, boardY + board.getSize() + 1);
				cputs("||");
				for (int i = 0; i < board.getSize(); i++)
				{
					printSide(i, i);
				}
			}
			redrawBoard = false;
		}
		redrawView = false;
		_setcursortype(_SOLIDCURSOR);
	}
	if (autoDetectEnd)
	{
		if (showImpossibleFields())
		{
			gotoxy(outputX, outputY+6);
			cputs("Nie da sie ukonczyc tej planszy!");
		}
	}
	setCursorOnBoard();
}

bool Game::loadGame(FILE* file)
{
	int autoDetectEndI, drawSidesI, evidentFieldsShownI;
	fscanf(file, "%d %d %d %d %d\n", &cursorX, &cursorY, &autoDetectEndI, &drawSidesI, &evidentFieldsShownI);
	autoDetectEnd = autoDetectEndI;
	drawSides = drawSidesI;
	evidentFieldsShown = evidentFieldsShownI;
	
	board.load(file);
	if (evidentFieldsShown)
	{
		redrawView = true;
		render();
		showEvidentFields();
		setCursorOnBoard();
		handleInput();
	}
	
	return true;
}

bool Game::saveGame(FILE* file)
{
	int autoDetectEndI = autoDetectEnd;
	int drawSidesI = drawSides;
	int evidentFieldsShownI = evidentFieldsShown;
	fprintf(file, "%d %d %d %d %d\n", cursorX, cursorY, autoDetectEndI, drawSidesI, evidentFieldsShownI);
	board.save(file);
	return true;
}

void Game::printInfo(int x, int y)
{

	const int size = 15;
	if (redrawView == true)
	{
	char* info[size] = {
	"strzalki\tporuszanie kursorem", 
	"esc\t\twyjscie z programu",
	"n\t\tnowa gra",
	"0/1\t\twpisanie cyfry na plansze",
	"o\t\tlosowe wypelnianie planszy",
	"p\t\tprosta podpowiedz",
	"r\t\tzmiana rozmiaru planszy",
	"k\t\tsprawdzanie mozliwosci ukonczenia gry",
	"d\t\tsprawdzenie reguly (2)",
	"a\t\tauto detekcja konca gry",
	"j\t\tpodswietlanie jednoznacznych pol",
	"w\t\twypelnienie jednoznacznych pol",
	"s\t\tzapisanie stanu gry",
	"l\t\todczytanie stanu gry"};
	
		gotoxy(x, y);
		cputs("      Krzysztof Dubanowicz  172029");
		printLine(x, y + 1, infoWidth, RightDir);
		for (int i = 0; i < size; i++)
		{
			gotoxy(x, y + 2 + i);
			cputs(info[i]);
		}
		printLine(x, y + size + 2, infoWidth, RightDir);
		printLine(x, y + size + 11, infoWidth, RightDir);
	}
	if(autoDetectEnd)
	{
		gotoxy(x, y + 11);
		textbackground(LIGHTGREEN);
		textcolor(BLACK);
		putch('a');
		textcolor(LIGHTGRAY);
		textbackground(BLACK);
	}
	sprintf_s(textBuffor, "Kursor(x, y): (%2d, %2d)", cursorX, cursorY);
	gotoxy(x, y + size + 3);
	cputs(textBuffor);
	clearOutputArea();
	
	redrawInfo = false;
}

void Game::printBorder(const int x, const int y, const int width, const int height)
{
	//Corners
	gotoxy(x, y);
	putch('+');
	gotoxy(x + width - 1, y);
	putch('+');
	gotoxy(x, y + height - 1);
	putch('+');
	gotoxy(x + width - 1, y + height - 1);
	putch('+');
	//Top line
	printLine(x + 1, y, width - 2, RightDir);
	//Bottom line
	printLine(x + 1, y + height - 1, width - 2, RightDir);
	//Left line
	printLine(x, y + 1, height - 2, DownDir);
	//Right line
	printLine(x + width - 1, y + 1, height - 2, DownDir);
}

void Game::printLine(const int x, const int y, const int length, const Direction dir)
{
	
	if(dir == RightDir)
	{
		gotoxy(x, y);
		for(int i = 0 ; i < length; i++)
		{
			putch('-');
		}
	}
	else if(dir == DownDir)
	{
		for(int i = 0; i < length; i++)
		{
			gotoxy(x, y + i);
			putch('|');
		}
	}
}

void Game::printSide(int x, int y)
{
	const int onesInRow = board.countInRow(y, 1);
	const int zerosInRow = board.countInRow(y, 0);
	const int onesInCol = board.countInCol(x, 1);
	const int zerosInCol = board.countInCol(x, 0);
	const bool wideNumber = board.getSize() >= 20;
	char number[3];
	char format[5];
	sprintf(format, "%s", (wideNumber ? "%02d" : "%01d"));
	sprintf(number, format, onesInRow);
	gotoxy(boardX - 3 + !wideNumber, boardY + y);
	cputs(number);
	sprintf(number, format, zerosInRow);
	gotoxy(boardX + board.getSize() + 1, boardY + y);
	cputs(number);
	sprintf(number, format, onesInCol);
	gotoxy(boardX + x, boardY-3 + !wideNumber);
	putch(number[0]);
	gotoxy(boardX + x, boardY - 2);
	if(wideNumber)
		putch(number[1]);
	sprintf(number, format, zerosInCol);
	gotoxy(boardX + x, boardY + board.getSize()+1);
	putch(number[0]);
	gotoxy(boardX + x, boardY + board.getSize() + 2);
	if(wideNumber)
		putch(number[1]);
}

void Game::clearArea(int x, int y, int width, int height)
{
	sprintf_s(textBuffor, "%*c", width, ' ');
	for (int i = 0; i < height; i++)
	{
		gotoxy(x, y + i);
		cputs(textBuffor);
	}
}

void Game::clearInputArea()
{
	clearArea(inputX, inputY, infoWidth, 1);
}

void Game::clearOutputArea()
{
	clearArea(outputX, outputY, infoWidth, 7);
}

void Game::showHint()
{
	gotoxy(outputX, outputY);
	Field field = board.getField(cursorX, cursorY);
	if(field.isLocked)
	{
		cputs("Pole jest niemodyfikowalne!");
	}
	else
	{
		bool zero = true;
		bool one = true;
		int line = 1;
		gotoxy(outputX, outputY + line);
		if (!board.isValidRule1(cursorX, cursorY, 1, textBuffor, getLength(textBuffor)))
		{
			cputs(textBuffor);
			line++;
			one = false;
		}
		if(!board.isValidRule1(cursorX, cursorY, 0, textBuffor, getLength(textBuffor)))
		{
			gotoxy(outputX, outputY + line);
			cputs(textBuffor);
			line++;
			zero = false;
		}
		if (!board.isValidRule2(cursorX, cursorY, 1, textBuffor, getLength(textBuffor)))
		{
			gotoxy(outputX, outputY + line);
			cputs(textBuffor);
			line++;
			one = false;
		}
		if (!board.isValidRule2(cursorX, cursorY, 0, textBuffor, getLength(textBuffor)))
		{
			gotoxy(outputX, outputY + line);
			cputs(textBuffor);
			line++;
			zero = false;
		}
		if (!board.isValidRule3(cursorX, cursorY, 1, textBuffor, getLength(textBuffor)))
		{
			gotoxy(outputX, outputY + line);
			cputs(textBuffor);
			line++;
			one = false;
		}
		if (!board.isValidRule3(cursorX, cursorY, 0, textBuffor, getLength(textBuffor)))
		{
			gotoxy(outputX, outputY + line);
			cputs(textBuffor);
			line++;
			zero = false;
		}
		gotoxy(outputX, outputY);
		if (zero || one)
		{
			sprintf_s(textBuffor, "%s %s%s", "Mozna wpisac:", (zero ? "0 " : ""), (one ? "1" : ""));
			cputs(textBuffor);
		}
		else
		{
			cputs("Nie mozna wpisac zadnej cyfry.");
		}

	}

}

bool Game::showImpossibleFields()
{
	bool result = false;
	char empty = board.getEmptyChar();
	for(int y = 0; y < board.getSize(); y++)
	{
		for(int x = 0; x < board.getSize(); x++)
		{
			if(board.getField(x, y).isEmpty)
			{
				if(board.canSetField(x, y, 1) == false && 
				   board.canSetField(x, y, 0) == false)
				{
					gotoxy(boardX + x, boardY + y);
					textbackground(LIGHTRED);
					putch(empty);
					textbackground(BLACK);
					result = true;
				}
			}
		}
	}
	impossibleFieldsShown = true;
	return result;
}

void Game::showEvidentFields()
{
	evidentFields.clear();
	for(int y = 0; y < board.getSize(); y++)
	{
		for(int x = 0; x < board.getSize(); x++)
		{
			if(board.getField(x, y).isEmpty)
			{
				bool putOne = board.canSetField(x, y, 1);
				bool putZero = board.canSetField(x, y, 0);
				if(putOne != putZero)
				{
					gotoxy(boardX + x, boardY + y);
					textcolor(LIGHTGREEN);
					putch('0' + putOne);
					evidentFields.add({x, y, (int)putOne});
				}
			}
		}
	}
	textcolor(LIGHTGRAY);
}

void Game::setEvidentFields()
{
	ListElement<evidentField>* field = evidentFields.get(0);
	for(int i = 0; i < evidentFields.getSize(); i++)
	{
		board.trySetField(field->value.x, field->value.y, field->value.value);
		field = field->next;
	}
}

void Game::handleInput()
{
	while (!input.updateAction());

	if (impossibleFieldsShown)
	{
		impossibleFieldsShown = false;
		redrawInfo = true;
		redrawBoard = true;
	}

	switch(input.getAction())
	{
	case Exit:
		running = false;
		break;
	case Left:
		cursorX += (cursorX <= 0 ? 0: -1);
		redrawInfo = true;
		break;
	case Right:
		cursorX += (cursorX >= board.getSize()-1 ? 0 : 1);
		redrawInfo = true;
		break;
	case Down:
		cursorY += (cursorY >= board.getSize()-1 ? 0 : 1);
		redrawInfo = true;
		break;
	case Up:
		cursorY += (cursorY <= 0 ? 0 : -1);
		redrawInfo = true;
		break;
	case One:
		if (board.trySetField(cursorX, cursorY, 1))
		{
			textcolor(LIGHTGRAY);
			putch('1');
			gotoxy(cursorX + boardX, cursorY + boardY);
			redrawBoard = true;
		}
		break;
	case Zero:
		if (board.trySetField(cursorX, cursorY, 0))
		{
			textcolor(LIGHTGRAY);
			putch('0');
			gotoxy(cursorX + boardX, cursorY + boardY);
			redrawBoard = true;
		}
		break;
	case NewGame:
		board.setNewGame();
		cursorX = 0;
		cursorY = 0;
		setCursorOnBoard();
		redrawView = true;
		break;
	case RandomFill:
		board.setRandomBoard();
		redrawBoard = true;
		break;
	case ChangeSize:
	{
		gotoxy(inputX, inputY);
		cputs("Podaj rozmiar: ");
		int size = Input::getUInt();

		if (size > 0 && size <= 24 && size % 2 == 0)
		{
			board.setBoardSize(size);
			char fileName[40];
			sprintf(fileName, "board%d.txt", size);
			FILE* file = fopen(fileName, "r");
			if(file != nullptr)
			{
				if(board.loadBoardFromFile(file) == false)
					board.setRandomBoard();

				fclose(file);
			}
			else
				board.setRandomBoard();

			cursorX = 0;
			cursorY = 0;
		}
		redrawView = true;
	}
		break;
	case SimpleHint:
		showHint();
		redrawInfo = false;
		break;
	case ShowImpossibleFields:
		if (showImpossibleFields())
		{
			gotoxy(outputX, outputY+6);
			cputs("Nie da sie ukonczyc tej planszy!");
		}
		break;
	case ShowSides:
		drawSides = !drawSides;
		if(drawSides == false)
		{
			clearArea(boardX - 3, boardY - 3, 2, board.getSize() + 3);
			clearArea(boardX+board.getSize()+1, boardY-3, 3, board.getSize()+6);
			clearArea(boardX, boardY + board.getSize() + 1, board.getSize()+1, 3);
			clearArea(boardX-1, boardY - 3, board.getSize()+1, 2);
		}
		redrawBoard = true;
		break;
	case AutoDetectEnd:
		autoDetectEnd = !autoDetectEnd;
		drawSides = true;
		redrawView = true;
		if (autoDetectEnd == false)
			drawSides = false;
		break;
	case EvidentFields:
		evidentFieldsShown = true;
		showEvidentFields();
		setCursorOnBoard();
		handleInput();
		break;
	case SaveGame:
	{
		char fileName[100];
		gotoxy(inputX, inputY);
		cputs("Podaj nazwe pliku: ");
		Input::getStr(fileName, getLength(fileName));
		FILE* file = fopen(fileName, "w");
		if(file != nullptr)
		{
			if(saveGame(file))
			{
				gotoxy(outputX, outputY+6);
				cputs("Udalo sie zapisac gre!");
			}
			else
			{
				gotoxy(outputX, outputY+6);
				cputs("Udalo sie zapisac gre!");
			}
			fclose(file);
		}
		clearInputArea();
	}
		break;
	case LoadGame:
	{
		char fileName[100];
		gotoxy(inputX, inputY);
		cputs("Podaj nazwe pliku: ");
		Input::getStr(fileName, getLength(fileName));
		FILE* file = fopen(fileName, "r");
		if(file != nullptr)
		{
			if(loadGame(file))
			{
				redrawView = true;
			}
			else
			{
				gotoxy(outputX, outputY + 6);
				cputs("Nie udalo sie wczytac gry!");
			}
			fclose(file);
		}
		else
		{
			gotoxy(outputX, outputY+6);
			cputs("Nie udalo sie otworzyc pliku!");
		}
		clearInputArea();
	}
		break;
	}

	if (evidentFieldsShown)
	{
		evidentFieldsShown = false;
		if (input.getAction() == SetEvidentFields)
		{
			setEvidentFields();
		}
		redrawBoard = true;
	}
}

void Game::setCursorOnBoard()
{
	gotoxy(boardX + cursorX, boardY + cursorY);
}
