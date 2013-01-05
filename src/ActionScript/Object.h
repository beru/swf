#pragma once

#include <string>
#include <map>

namespace SWF {
namespace ActionScript {

struct Object
{
	Object();
	virtual ~Object() {}
	
	Object& get(const std::string& name);
	const Object& getMember(const std::string& name) const;
	void setMember(const std::string& name, const Object& o);
	void deleteMember(const std::string& name);
	bool in(const std::string& name) const;
	void callMethod(const std::string& name);
	
	bool isInstanceOf(const Object& constr);
	
/*
	bool addProperty(const std::string& name, const Function& getter, const Function& setter);
	bool hasOwnProperty(const std::string& name) const;
	bool isPropertyEnumerable(const std::string& name) const;
	bool isPrototypeOf(const Object& o) const;
	static bool registerClass(const std::string& name, const Function& theClass);
	String toString() const;
	bool unwatch(const std::string& name);
*/
protected:
	typedef std::map<std::string, Object> ContainerT;
	ContainerT aa;
};

} // namespace ActionScript
} // namespace SWF
