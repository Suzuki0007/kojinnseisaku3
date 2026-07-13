#include "pch.h"
#include "battlecommandmenu.h"

BattleCommandMenu::BattleCommandMenu()
{
	_entries.push_back({ BattleCommand::Fight, "たたかう" });
	_entries.push_back({ BattleCommand::End, "終了" });
}

void BattleCommandMenu::MoveUp()
{
	if(_cursor <= 0)
	{
		_cursor = static_cast<int>(_entries.size()) - 1;
	}
	else
	{
		--_cursor;
	}
}

void BattleCommandMenu::MoveDown()
{
	if(_cursor >= static_cast<int>(_entries.size()) - 1)
	{
		_cursor = 0;
	}
	else
	{
		++_cursor;
	}
}

void BattleCommandMenu::Render(int x, int y, int lineHeight) const
{
	for(int i = 0; i < static_cast<int>(_entries.size()); ++i)
	{
		// ★ カーソルマークの文字列の頭にも L を付けます
		const char* cursorMark = (i == _cursor) ? "> " : "   "; //
		DrawFormatString(x, y + i * lineHeight, GetColor(255, 255, 255),
			"%s%s", cursorMark, _entries[i].label.c_str()); //
	}
}