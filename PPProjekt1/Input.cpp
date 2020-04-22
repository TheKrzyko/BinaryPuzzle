
#include "Input.h"
#include "Board.h"


Input::Input()
{
}

Input::~Input()
{
}

void Input::add(Action action, int code0, int code1)
{
	actions[actionsSize] = {code0, code1, action};
	actionsSize++;
}

void Input::add(Action action, int code0)
{
	add(action, code0, 0);
}

unsigned int Input::getUInt()
{
	const int maxLength = 10;
	char str[maxLength];
	char ch;
	int length = 0;
	while((((ch = getch()) >= '0' && ch <= '9') || ch == '\b') && length < maxLength)
	{
		if(ch == '\b' && length > 0)
		{
			length--;
			putch('\b');
			putch(' ');
			putch('\b');
		}
		else if(ch != '\b')
		{
			str[length] = ch;
			length++;
			putch(ch);
		}
	}
	unsigned int result = 0;
	for(int i = 0; i < length; i++)
	{
		result *= 10;
		result += str[i] - '0';
	}
	return result;
}

void Input::getStr(char* str, size_t size)
{
	int pos = 0;
	char ch;
	while((ch = getch()) != '\r' && ch != ' ' && pos < size-1)
	{
		if (ch == '\b' && pos > 0)
		{
			pos--;
			putch('\b');
			putch(' ');
			putch('\b');
		}
		else if (ch != '\b')
		{
			str[pos] = ch;
			putch(ch);
			pos++;
		}
	}
	str[pos] = '\0';
}

void Input::getStrLine(char* str, size_t size)
{
	int pos = 0;
	char ch;
	while((ch = getch()) != '\r' && pos < size-1)
	{
		if (ch == '\b' && pos > 0)
		{
			pos--;
			putch('\b');
			putch(' ');
			putch('\b');
		}
		else if (ch != '\b')
		{
			str[pos] = ch;
			putch(ch);
			pos++;
		}
	}
	str[pos] = '\0';
}

bool Input::updateAction()
{
	int code0;
	int code1 = 0;
	code0 = getch();
	if (code0 == 0)
		code1 = getch();
	for (int i = 0; i < actionsSize; i++)
	{
		
		if(code0 != 0 && actions[i].code0 != 0)
		{
			if(actions[i].code0 == code0 || actions[i].code1 == code0)
			{
				currentAction = actions[i].action;
				return true;
			}
		}
		else if (actions[i].code0 == code0 && actions[i].code1 == code1)
		{
			currentAction = actions[i].action;
			return true;
		}
	}
	return false;
}

Action Input::getAction()
{
	return currentAction;
}

