#pragma once
#include "mapbase.h"

class SkySphere : public MapBase
{
public:
	SkySphere() = default;
	virtual ~SkySphere() = default;

	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Process() override;
	virtual bool Render() override;
};

