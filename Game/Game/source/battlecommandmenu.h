#pragma once
#include "pch.h"

class BattleCommandMenu
{
public:
	enum class BattleCommand
	{
		Fight,	// たたかう
		End,		// 終了
		_EOT_
	};

	BattleCommandMenu();

	void MoveUp();

	void MoveDown();

	BattleCommand GetSelectedCommand() const
	{
		return _entries[_cursor].command;
	}

	int GetCursor() const { return _cursor; }

	void Render(int x, int y, int lineHeight) const;

private:
	struct Entry
	{
		BattleCommand command;
		std::string label;
	};

	std::vector<Entry> _entries;
	int _cursor{ 0 };
};

