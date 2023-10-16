#pragma once

#include "Handle.h"

#include <queue>
#include <vector>

template <typename T>
class HandlePool
{
public:
	~HandlePool()
	{
		for (auto& resource : resources)
			if (resource.first)
				resource.first->Release();
	}

	Handle CreateHandle(T& resource)
	{
		// Find an index
		if (availableIndices.empty())
		{
			// No more indices, add new one and return it's value
			auto index = resources.size();
			resources.push_back(std::make_pair(&resource, 0));
			return Handle(index, 0);
		}
		else
		{
			// Get the next available index and insert
			auto& index = availableIndices.front();
			availableIndices.pop();

			// Increment the version
			index.second++;
			resources[index.first] = std::make_pair(&resource, index.second);

			return Handle(index.first, index.second);
		}
	}

	void ReleaseHandle(const Handle& handle)
	{
		auto& data = resources.at(handle.GetIndex());
		data.first->Release();
		data.first = nullptr;
		availableIndices.push(std::make_pair(handle.GetIndex(), handle.GetVersion()));
	}

	T& GetResource(const Handle& handle) const
	{
		return *resources.at(handle.GetIndex()).first;
	}


private:
	std::vector<std::pair<T*, int>> resources;
	std::queue<std::pair<int, int>> availableIndices;
};