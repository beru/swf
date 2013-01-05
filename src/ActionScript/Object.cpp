
#include "Object.h"

namespace SWF {
namespace ActionScript {

Object::Object()
{
}

Object& Object::get(const std::string& name)
{
	return aa[name];
}

const Object& Object::getMember(const std::string& name) const
{
	ContainerT::const_iterator it = aa.find(name);
	return it->second;
}

void Object::setMember(const std::string& name, const Object& o)
{
	aa[name] = o;
}

void Object::deleteMember(const std::string& name)
{
	aa.erase(name);
}

bool Object::in(const std::string& name) const
{
	return aa.find(name) != aa.end();
}

void Object::callMethod(const std::string& name)
{
}

bool Object::isInstanceOf(const Object& constr)
{
	return true;
}

} // namespace ActionScript
} // namespace SWF

