#pragma once
enum Action
{
	None,
	Exit,
	NewGame,
	Right,
	Up,
	Left,
	Down,
	Zero,
	One,
	RandomFill,
	SimpleHint,
	ChangeSize,
	ShowImpossibleFields,
	ShowSides,
	AutoDetectEnd,
	EvidentFields,
	SetEvidentFields,
	SaveGame,
	LoadGame
};

struct KeyAction
{
	int code0 = 0;
	int code1 = 0;
	Action action = None;
};

class Input
{
	Action currentAction = None;
	KeyAction actions[40];
	int actionsSize = 0;

public:
	Input();
	~Input();
	void add(Action action, int code0, int code1);
	void add(Action action, int code0);

	bool updateAction();
	Action getAction();

	static unsigned int getUInt();
	static void getStr(char* str, size_t size);
	static void getStrLine(char* str, size_t size);
};

