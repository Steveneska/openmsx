#ifndef MICROSOLFDC_HH
#define MICROSOLFDC_HH

#include "WD2793BasedFDC.hh"

namespace openmsx {

class MicrosolFDC final : public WD2793BasedFDC
{
public:
	explicit MicrosolFDC(DeviceConfig& config);

	[[nodiscard]] byte readIO(uint16_t port, EmuTime time) override;
	[[nodiscard]] byte peekIO(uint16_t port, EmuTime time) const override;
	void writeIO(uint16_t port, byte value, EmuTime time) override;

	template<typename Archive>
	void serialize(Archive& ar, unsigned version);
};

} // namespace openmsx

#endif
