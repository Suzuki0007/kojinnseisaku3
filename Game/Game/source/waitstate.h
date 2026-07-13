#pragma once
#include "charabase.h"

class WaitState
{
public:
	WaitState() = default;
	~WaitState() = default;
	
	void Enter(CharaBase* owner);
	void Update(CharaBase* owner);
	void Exit(CharaBase* owner);
};

