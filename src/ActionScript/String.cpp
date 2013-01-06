
#include "String.h"

namespace SWF {
namespace ActionScript {

// static
Object* String::New()
{
	Object* p = new String();
	return p;
}

} // namespace ActionScript
} // namespace SWF
