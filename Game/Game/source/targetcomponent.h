#pragma once
#include "charabase.h"
#include "targetstrategy.h"
#include "targetprovider.h"

namespace target
{
	constexpr float K_MIN_DISTANCE = 0.0001f; // 最小距離
}

class TargetComponent : public Component<CharaBase>
{
public:
	using base = Component<CharaBase>;

	enum class CycleDirection
	{
		Left,
		Right
	};

	explicit TargetComponent
	(
		CharaBase& owner,
		std::unique_ptr<ITargetStrategy> strategy,
		std::unique_ptr<ITargetProvider> provider
	);

	virtual ~TargetComponent() = default;

	bool Render() override;

	void SetCamera(Camera* cam) { _camera = cam; }

	// ターゲットの取得
	bool AcquireTarget();

	// ターゲットの切り替え
	bool CycleTarget(CycleDirection dir);

	// ターゲット候補の更新
	void RefreshCandidate();

	Vec4 FaceTarget(const Vec4& currentDir, float t = 1.0f) const;

	CharaBase* GetTarget() const { return _target; }
	bool HasTarget() const { return _target != nullptr; }
	void ClearTarget() { _target = nullptr; }

protected:
	std::unique_ptr<ITargetStrategy> _strategy;
	std::unique_ptr<ITargetProvider> _provider;
	std::vector<TargetInfo> _candidate;
	CharaBase* _target{ nullptr };
	Camera* _camera{ nullptr };
};

