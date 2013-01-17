
#include "setting.h"

#include "xml_serialization/serialization.h"

namespace XMLBinding {

	template <typename T>
	struct Connector : public T
	{
		void bind(Setting& v) {
			bindToAttribute("addFilePathAndLineNo", v.addFilePathAndLineNo);
			bindToAttribute("useOnlyFileName", v.useOnlyFileName);
			bindToAttribute("useFileNameFiltering", v.useFileNameFiltering);
			bindContainer<Connector>("TargetFileName", v.targetFileNames);
		}
		
		void bind(std::string& v) {
			bindToAttribute("name", v);
		}
	};

} // namespace XMLBinding

bool Load(Setting& s, const char* filePath)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filePath);
	if (result) {
		XMLBinding::Connector<XMLBinding::Reader> reader;
		reader.node_ = doc.first_child();
		reader.bind(s);
		return true;
	}else {
		return false;
	}
}

bool Save(const Setting& s, const char* filePath)
{
	pugi::xml_document doc;
	XMLBinding::Connector<XMLBinding::Writer> writer;
	writer.node_ = doc.append_child(pugi::node_element);
	writer.node_.set_name("Setting");
	writer.bind((Setting&) s);
	doc.save_file(filePath);
	return true;
}

