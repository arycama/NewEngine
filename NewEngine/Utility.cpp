#include <stdexcept>
#include <comdef.h>

#include "Utility.h"

using namespace std;

inline void Utility::ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
		throw _com_error(hr);
}
