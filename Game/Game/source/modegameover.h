#pragma once

class ModeGameOver :
    public ModeBase
{
	using base = ModeBase;
public:
	ModeGameOver() = default;
	virtual ~ModeGameOver() = default;
	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Process() override;
	virtual bool Render() override;
};

