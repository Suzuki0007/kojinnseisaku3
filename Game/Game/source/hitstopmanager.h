#pragma once
#include "hitinfo.h"

namespace hit
{
	constexpr float HIT_STOP_FRAME = 120.0f;
}

class HitStopManager : public ObserverBase<HitInfo>
{
public:
	HitStopManager(Subject<HitInfo>& hitSubject) : ObserverBase<HitInfo>(hitSubject) {}

	// ヒット情報を受け取ったときの処理
	void OnNotify(const HitInfo& info) override
	{
		_remainingFrame = max::MyMax(_remainingFrame, static_cast<int>(info.hitStopDuration * hit::HIT_STOP_FRAME));
	}

	// ヒットストップの更新処理
	void Update()
	{
		if(_remainingFrame > 0)
		{
			_remainingFrame--;
			_isStopping = true;
		}
		else
		{
			_isStopping = false;
		}
	}

	bool IsStopping() const { return _isStopping; }
	float GetTimeScale() const
	{
		if(_remainingFrame > 0)
		{
			return 0.0f; // ヒットストップ中は時間を止める
		}
		else
		{
			return 1.0f; // 通常の時間スケール
		}
	}

protected:
	int _remainingFrame{ 0 };
	bool _isStopping{ false };
};

