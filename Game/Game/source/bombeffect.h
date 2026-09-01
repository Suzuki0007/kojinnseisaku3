#include "effectbase.h"

class BombEffect final : public EffectBase
{
	using base = EffectBase;

public:
	BombEffect() = default;
	virtual ~BombEffect() = default;
	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Process() override;
	virtual bool Render() override;

	virtual const char* GetCharaClassName() const override;
	virtual float GetSpeed() const override;
	virtual void CommandAttack(IBattleReceiver* target) override;
	virtual bool IsExceutionAction() const override;

	void SetUp(const Vec4& startpos, const Vec4& dir, float distance) override;


	bool PlayEffect(const Vec4& pos);
};