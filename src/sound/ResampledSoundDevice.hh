#ifndef RESAMPLEDSOUNDDEVICE_HH
#define RESAMPLEDSOUNDDEVICE_HH

#include "SoundDevice.hh"

#include "DynamicClock.hh"
#include "EnumSetting.hh"

#include "Observer.hh"

#include <cstdint>
#include <memory>

namespace openmsx {

class MSXMotherBoard;
class ResampleAlgo;
class Setting;

class ResampledSoundDevice : public SoundDevice, protected Observer<Setting>
{
public:
	enum class ResampleType : uint8_t { HQ, BLIP };

	/** Note: To enable various optimizations (like SSE), this method is
	  * allowed to generate up to 3 extra sample.
	  * @see SoundDevice::updateBuffer()
	  */
	bool generateInput(float* buffer, size_t num);

	[[nodiscard]] DynamicClock& getEmuClock() { return emuClock; }

	// setBalance() might switch between mono/stereo
	void postSetBalance() override {
		createResampler();
		SoundDevice::postSetBalance();
	}

protected:
	ResampledSoundDevice(MSXMotherBoard& motherBoard, std::string_view name,
	                     static_string_view description, unsigned channels,
	                     unsigned inputSampleRate, bool stereo);
	~ResampledSoundDevice();

	// SoundDevice
	void setOutputRate(unsigned hostSampleRate, double speed) override;
	bool updateBuffer(size_t length, float* buffer,
	                  EmuTime time) override;

	// Observer<Setting>
	void update(const Setting& setting) noexcept override;

	void createResampler();

private:
	EnumSetting<ResampleType>& resampleSetting;
	std::unique_ptr<ResampleAlgo> algo;
	DynamicClock emuClock{EmuTime::zero()}; // time of the last produced emu-sample,
	                                        //    ticks once per emu-sample
};

} // namespace openmsx

#endif
