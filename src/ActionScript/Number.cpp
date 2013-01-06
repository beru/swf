
#include "Number.h"

namespace SWF {
namespace ActionScript {

// static
Object* Number::New()
{
	Object* p = new Number();
	return p;
}

} // namespace ActionScript
} // namespace SWF
