#include "pch.h"
#include "timemanager.h"


void TimeManager::Start()
{
	_state = TimeState::Play;
	_startTime = Clock::now();// タイマー開始時間を現在の時間に設定
	_elapsed = 0;// 経過時間を0にリセット
	_pausedAccum = std::chrono::milliseconds(0);// 一時停止時間の累積を0にリセット
	_pauseStart = TimePoint();// 一時停止開始時間を初期化
}

void TimeManager::Stop()
{
	if(_state == TimeState::Play)
	{
		Update();// 経過時間を更新
	}
	_state = TimeState::Stop;// タイマー状態を停止に設定
}

void TimeManager::Pause()
{
	if(_state == TimeState::Play)
	{
		_state = TimeState::Pause;// タイマー状態を一時停止に設定
		_pauseStart = Clock::now();// 一時停止開始時間を現在の時間に設定
	}
}

void TimeManager::Resume()
{
	auto now = Clock::now();

	if(_state == TimeState::Pause)
	{
		_pausedAccum += std::chrono::duration_cast<MS>(now - _pauseStart);
		_state = TimeState::Play;// タイマー状態を再生に設定
	}
	else if(_state == TimeState::Stop)
	{
		_startTime = now - MS(_elapsed);// タイマー開始時間を現在の時間から経過時間を引いた値に設定
		_pausedAccum = MS(0);// 一時停止時間の累積を0にリセット
		_state = TimeState::Play;// タイマー状態を再生に設定
	}
}

void TimeManager::Update()
{
	if(_state == TimeState::Play)
	{
		auto now = Clock::now();
		auto totalDuration = (now - _startTime) - _pausedAccum;// タイマー開始から現在までの時間から一時停止していた時間を引いた値を計算
		_elapsed = static_cast<uint64_t>(std::chrono::duration_cast<MS>(totalDuration).count());// 経過時間をミリ秒で更新
	}
}

void TimeManager::ResumeFrom(uint64_t targetElapsed)
{
	auto now = Clock::now();

	_elapsed = targetElapsed;// 経過時間を指定された値に設定

	_startTime = now - MS(_elapsed);// タイマー開始時間を現在の時間から経過時間を引いた値に設定

	_pausedAccum = MS(0);// 一時停止時間の累積を0にリセット
	_pauseStart = TimePoint();// 一時停止開始時間を初期化
	_state = TimeState::Play;// タイマー状態を再生に設定
}