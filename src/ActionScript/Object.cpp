
#include "Object.h"

namespace SWF {
namespace ActionScript {

Object::Object()
{
}

// static
Object* Object::New()
{
	return new Object();
}

bool Object::HasMember(const std::string& name) const
{
	return aa.find(name) != aa.end();
}

Object* Object::GetMember(const std::string& name)
{
	return aa[name];
}

const Object* Object::GetMember(const std::string& name) const
{
	ContainerT::const_iterator it = aa.find(name);
	return it->second;
}

void Object::SetMember(const std::string& name, Object* o)
{
	aa[name] = o;
}

void Object::DeleteMember(const std::string& name)
{
	aa.erase(name);
}

void Object::CallMethod(const std::string& name)
{
}

bool Object::IsInstanceOf(const Object& constr)
{
	return true;
}

} // namespace ActionScript
} // namespace SWF

