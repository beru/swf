
#include "Function.h"

namespace SWF {
namespace ActionScript {

// static
Object* Function::New()
{
	Object* p = new Function();
	return p;
}

} // namespace ActionScript
} // namespace SWF
