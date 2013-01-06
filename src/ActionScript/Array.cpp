
#include "Array.h"

namespace SWF {
namespace ActionScript {

// static
Object* Array::New()
{
	Object* p = new Array();
	return p;
}

} // namespace ActionScript
} // namespace SWF
