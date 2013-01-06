
#include "Date.h"

namespace SWF {
namespace ActionScript {

// static
Object* Date::New()
{
	Object* p = new Date();
	return p;
}

} // namespace ActionScript
} // namespace SWF
