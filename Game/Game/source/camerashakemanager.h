#pragma once
#include "hitinfo.h"
#include "camerashake.h"

class CameraShakeManager : public ObserverBase<HitInfo>
{
public:
	explicit CameraShakeManager(Subject<HitInfo>& subject);

	void OnNotify(const HitInfo& data) override;
	void Update() { _cameraShake.Update(); }
	Vec4 GetOffset() const { return _cameraShake.GetOffset(); }
	bool IsActive() const { return _cameraShake.IsActive(); }

private:
	CameraShake _cameraShake;
};

