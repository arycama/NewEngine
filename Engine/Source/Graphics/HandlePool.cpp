#include "Handle.h"
#include "HandlePool.h"

using namespace std;

template<typename T> Handle HandlePool<T>::CreateHandle(const T& resource)
{
	// Find an index
	if (availableIndices.empty())
	{
		// No more indices, add new one and return it's value
		auto index = resources.size();
		resources.push_back(make_pair(resources, 0));
		return Handle(index, 0);
	}
	else
	{
		// Get the next available index and insert
		auto& index = availableIndices.front();
		availableIndices.pop();

		// Increment the version
		index.second++;
		resources[index.first] = make_pair(resources, index.second);

		return Handle(index.first, index.second);
	}
}

template<typename T>
HandlePool<T>::HandlePool()
{
}

template<typename T> HandlePool<T>::~HandlePool()
{
	for (auto& resource : resources)
		if(resource.first)
			resource.first->Release();
}

template <typename T> void HandlePool<T>::ReleaseHandle(const Handle& handle)
{
	auto& data = resources.at(handle.GetIndex());
	data.first->Release();
	data.first = nullptr;
	availableIndices.push(make_pair(handle.GetIndex(), handle.GetVersion()));
}

template<typename T>
T& HandlePool<T>::GetResource(const Handle& handle) const
{
	return *resources.at(handle.GetIndex()).first;
}
