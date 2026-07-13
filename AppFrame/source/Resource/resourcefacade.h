#pragma once
#include "resourceserver.h"
#include "pch.h"

class ResourceFacade
{
public:
	ResourceFacade() = delete;
	static int GetModelHandle(const std::string_view filePath)
	{
		return ResourceServer::GetHandle<DxLib::MV1LoadModel>(filePath);
	}
	static int GetLoadHandle(const std::string_view filePath)
	{
		return ResourceServer::GetHandle<DxLib::LoadGraph>(filePath);
	}
};

namespace Load
{
	inline auto ImageGraph = ResourceFacade::GetLoadHandle;
	inline auto LoadModel = ResourceFacade::GetModelHandle;
}

