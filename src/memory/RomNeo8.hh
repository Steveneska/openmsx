#ifndef ROMNEO8_HH
#define ROMNEO8_HH

#include "RomBlocks.hh"

namespace openmsx {

class RomNeo8 final : public Rom8kBBlocks
{
public:
	RomNeo8(const DeviceConfig& config, Rom&& rom);

	void reset(EmuTime time) override;
	void writeMem(uint16_t address, byte value, EmuTime time) override;
	[[nodiscard]] byte* getWriteCacheLine(uint16_t address) override;

	template<typename Archive>
	void serialize(Archive& ar, unsigned version);

private:
	std::array<uint16_t, 6> blockReg;
};

} // namespace openmsx

#endif
