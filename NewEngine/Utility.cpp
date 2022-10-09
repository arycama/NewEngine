#include "Utility.h"
#include <stdexcept>

using namespace std;

void Utility::ThrowIfFailed(HRESULT hresult, const std::string& message)
{
	if (FAILED(hresult))
		throw runtime_error(message);
}
