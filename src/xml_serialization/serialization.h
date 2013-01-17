#pragma once

#include "pugixml/pugixml.hpp"
#include <queue>
#include <assert.h>
#include <iostream>
#include <map>
#include <boost/type_traits.hpp>
#include <boost/shared_ptr.hpp>
#include "impl.h"

#undef bind

namespace XMLBinding {

struct Writer
{
	pugi::xml_node node_;
	
	Writer()
	{
	}
	
	template <typename T>
	void bindToAttribute(const char* name, T& value)
	{
		pugi::xml_attribute attr = node_.append_attribute(name);
		attr.set_value(ToString(value).c_str());
	}

	template <typename T>
	void bindToAttribute(const std::string& name, T& value)
	{
		bindToAttribute(name.c_str(), value);
	}

/*
	template <typename T>
	void bindToAttribute(const char* name, optional<T>& value)
	{
		if (value) {
			bindToAttribute(name, *value);
		}
	}
*/
	
	template <typename MapT>
	void bindToAttributes(MapT& map)
	{
		for (MapT::const_iterator it=map.begin(); it!=map.end(); ++it) {
			bindToAttribute(it->first, it->second);
		}
	}
	
	template <typename SubClassT, typename T>
	void bind(T& value, boost::true_type isPointerType)
	{
		if (value) {
			((SubClassT*)this)->bind(*value);
		}
	}
	
	template <typename SubClassT, typename T>
	void bind(T& value, boost::false_type isPointerType)
	{
		bind<SubClassT>(value);
	}

	template <typename SubClassT, typename T>
	void bind(T& value)
	{
		((SubClassT*)this)->bind(value);
	}

	template <typename SubClassT, typename T>
	void bind(boost::shared_ptr<T>& value)
	{
		if (value) {
			((SubClassT*)this)->bind(*value);
		}
	}
	
	template <typename SubClassT, typename ContainerT>
	void bindContainer(const char* tagName, ContainerT& container)
	{
		for (ContainerT::const_iterator it=container.begin(); it!=container.end(); ++it) {
			bindToTag<SubClassT>(tagName, *it);
		}
	}
	
	template <typename SubClassT, typename ValueT>
	void bindToTag(const char* name, ValueT& value)
	{
		pugi::xml_node backup = node_;
		node_ = node_.append_child(pugi::node_element);
		node_.set_name(name);
		bind<SubClassT>((boost::remove_const<ValueT>::type&) value, boost::is_pointer<ValueT>());
		node_ = backup;
	}
	
};

struct Reader
{
	template <typename T>
	void bindToAttribute(const char* name, T& value)
	{
		pugi::xml_attribute attr = node_.attribute(name);
		if (attr.empty()) {
			return;
		}
		convert(attr.value(), strlen(attr.value()), value);
	}

/*
	template <typename T>
	void bindToAttribute(const char* name, optional<T>& value)
	{
		pugi::xml_attribute attr = node_.attribute(name);
		if (attr.empty()) {
			return;
		}
		T nv;
		convert(attr.value(), strlen(attr.value()), nv);
		value = nv;
	}
*/
	
	template <typename MapT>
	void bindToAttributes(MapT& map)
	{
		for (pugi::xml_attribute attr=node_.first_attribute(); attr; attr=attr.next_attribute()) {
			bindToAttribute(attr.name(), map[attr.name()]);
		}
	}
	
	template <typename SubClassT, typename T>
	void bind(T& value, boost::true_type isPointerType)
	{
		((SubClassT*)this)->newBind(value);
	}
	
	template <typename SubClassT, typename T>
	void bind(T& value, boost::false_type isPointerType)
	{
		bind<SubClassT>(value);
	}

	template <typename SubClassT, typename T>
	void bind(boost::shared_ptr<T>& value)
	{
		T* ptr;
		((SubClassT*)this)->newBind(ptr);
		value = boost::shared_ptr<T>(ptr);
	}

	template <typename SubClassT, typename T>
	void bind(T& value)
	{
		((SubClassT*)this)->bind(value);
	}

	template <typename SubClassT, typename T>
	void bindToTag(const char* tagName, T& value)
	{
		pugi::xml_node backup = node_;
		node_ = node_.child(tagName);
		if (!node_.empty()) {
			bind<SubClassT>(value, boost::is_pointer<T>());
		}
		node_ = backup;
	}
	
	template <typename SubClassT, typename ContainerT>
	void bindContainer(const char* tagName, ContainerT& container)
	{
		std::back_insert_iterator<ContainerT> it = std::back_inserter(container);
		pugi::xml_node backup = node_;
		for (pugi::xml_node n=node_.child(tagName); n; n=n.next_sibling(tagName)) {
			node_ = n;
			ContainerT::value_type v;
			bind<SubClassT>(v, boost::is_pointer<ContainerT::value_type>());
//			container.push_back(v);
//			*it = v;
//			++it;
			*it++ = v;
		}
		node_ = backup;
	}
	
	pugi::xml_node node_;
};

} // namespace XMLBinding

