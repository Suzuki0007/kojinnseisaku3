#pragma once

struct ScreenMotionBlurData
{
	bool isActive = false;
};

class ScreenMotionBlur
{
public:
	void Start();
	void Stop();

	bool IsActive() const;

private:
	ScreenMotionBlurData _data;
};

