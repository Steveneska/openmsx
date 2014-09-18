#ifndef ROMINFOTOPIC_HH
#define ROMINFOTOPIC_HH

#include "InfoTopic.hh"

namespace openmsx {

class RomInfoTopic final : public InfoTopic
{
public:
	explicit RomInfoTopic(InfoCommand& openMSXInfoCommand);

	virtual void execute(array_ref<TclObject> tokens,
	                     TclObject& result) const;
	virtual std::string help(const std::vector<std::string>& tokens) const;
	virtual void tabCompletion(std::vector<std::string>& tokens) const;
};

} // namespace openmsx

#endif
