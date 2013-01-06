#pragma once

#include <string>
#include <map>

namespace SWF {
namespace ActionScript {

struct Object
{
	Object();
	virtual ~Object() {}

	static Object* New();
	
	bool HasMember(const std::string& name) const;
	Object* GetMember(const std::string& name);
	const Object* GetMember(const std::string& name) const;
	void SetMember(const std::string& name, Object* o);
	void DeleteMember(const std::string& name);
	void CallMethod(const std::string& name);
	
	bool IsInstanceOf(const Object& constr);
	
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
	typedef std::map<std::string, Object*> ContainerT;
	ContainerT aa;
};

} // namespace ActionScript
} // namespace SWF
