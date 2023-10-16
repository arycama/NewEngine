#include <queue>
#include <vector>

template <typename T>
class HandlePool
{
public:
	HandlePool();
	~HandlePool();
	Handle CreateHandle(const T& resource);
	void ReleaseHandle(const Handle& handle);

	T& GetResource(const Handle& handle) const;

private:
	std::vector<std::pair<T*, int>> resources;
	std::queue<std::pair<int, int>> availableIndices;
};