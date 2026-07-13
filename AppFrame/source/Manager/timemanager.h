#pragma once
#include "../singleton.h"
#include "DxLib.h"
#include <cstdint>
#include <chrono>

class TimeManager : public Singleton<TimeManager>
{
	friend class Singleton<TimeManager>;
public:
	enum class TimeState
	{
		Play,
		Stop,
		Pause
	};

	void Start();
	void Stop();
	void Pause();
	void Resume();
	void Update();
	void ResumeFrom(uint64_t targetElapsed);
	
	uint64_t GetElapsedMs() const { return _elapsed; }// 経過時間を秒で取得する関数

	TimeState GetState() const { return _state; }

private:
	TimeManager() = default;
	virtual ~TimeManager() = default;

	using Clock = std::chrono::steady_clock;// 絶対に逆戻りしない安定した時計
	using TimePoint = Clock::time_point;// ある特定の時間の瞬間を表す型
	using MS = std::chrono::milliseconds;// ミリ秒を表す型

	TimeState _state { TimeState::Stop };
	TimePoint _startTime;	// タイマーが開始された時間
	TimePoint _pauseStart;	// 一時停止を押した瞬間の時間

	MS _pausedAccum{ 0 };	// 一時停止していた時間の累積
	uint64_t _elapsed{ 0 };	// 経過時間（ミリ秒）
};

