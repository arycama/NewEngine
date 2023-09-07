//#pragma once
//
//#include <string>
//#include <unordered_map>
//
//template<class T>
//class AssetCache
//{
//private:
//	std::unordered_map<std::string, unique_ptr<T>> cache;
//
//public:
//	T Get(const std::string& path)
//	{
//		auto result = cache.find(path);
//		if (result != cache.end())
//			return result;
//	}
//
//
//};