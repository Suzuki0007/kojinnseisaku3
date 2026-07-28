#pragma once

class AnimationManager: public Singleton<AnimationManager>
{
	friend class Singleton<AnimationManager>;

public:
	struct Instance
	{
		int id { 0 };					// 管理用ID
		int handle { 0 };				// アニメーションハンドル
		int attachIndex { 0 };			// アタッチインデックス
		std::string_view name;			// アニメーション名
		float totalTime { 0.0f };		// 総再生時間
		float playTime { 0.0f };		// 再生時間
		float speed { 0.0f };			// 再生速度
		float speedVariance { 0.0f };   // 再生速度のばらつき
		bool loop { false };			// ループ再生するかどうか
		bool playing { false };			// 再生中フラグ
	};

	AnimationManager();
	~AnimationManager();

	// アニメーションの再生
	int Play(int handle, const std::string_view& name, bool loop = true, float speed = 1.0f, float speedVariance = 0.0f);

	// アニメーションを停止
	void Stop(int id);

	// 全てのモデルに関する全アニメーションを停止
	void StopAllModel(int handle);

	// 全てを停止
	bool Terminate();

	// 毎フレーム更新
	void Update(float time);

	// 再生時間を設定
	bool SetTime(int id, float time);

	// 一時停止/再開
	bool SetPlaying(int id, bool play);// play=true:再生、play=false:一時停止

	// 再生中かどうか
	bool IsPlaying(int id) const;

	// 総再生時間を取得
	float GetTotalTime(int id) const;

	float GetPlayTime(int id) const;
private:

	int CreateInstance(int handle, int attachindex, const std::string_view& name, float totaltime, bool loop, float speed, float speedVariance);
	void DetachInstance(int id);

protected:
	std::unordered_map<int, Instance> _animInstance;
	int _nextId { 0 };
};

