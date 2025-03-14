#ifndef ROMASCII16_2_HH
#define ROMASCII16_2_HH

#include "RomAscii16kB.hh"

#include <cstdint>

namespace openmsx {

class RomAscii16_2 final : public RomAscii16kB
{
public:
	enum class SubType : uint8_t { ASCII16_2, ASCII16_8 };
	RomAscii16_2(const DeviceConfig& config, Rom&& rom, SubType subType);

	void reset(EmuTime::param time) override;
	[[nodiscard]] byte readMem(word address, EmuTime::param time) override;
	[[nodiscard]] const byte* getReadCacheLine(word address) const override;
	void writeMem(word address, byte value, EmuTime::param time) override;
	[[nodiscard]] byte* getWriteCacheLine(word address) override;

	template<typename Archive>
	void serialize(Archive& ar, unsigned version);

private:
	byte sramEnabled;
};

} // namespace openmsx

#endif
