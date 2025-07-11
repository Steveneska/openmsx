#include "MSXFacMidiInterface.hh"
#include "MidiInDevice.hh"
#include "outer.hh"
#include "serialize.hh"

namespace openmsx {

MSXFacMidiInterface::MSXFacMidiInterface(const DeviceConfig& config)
	: MSXDevice(config)
	, MidiInConnector(MSXDevice::getPluggingController(), MSXDevice::getName() + "-in")
	, outConnector   (MSXDevice::getPluggingController(), MSXDevice::getName() + "-out")
	, i8251(getScheduler(), interface, getCurrentTime())
{
	EmuTime time = getCurrentTime();
	static constexpr auto period = EmuDuration::hz(500000); // 500 kHz
	i8251.getClockPin().setPeriodicState(period, period / 2, time);
	reset(time);
}

void MSXFacMidiInterface::reset(EmuTime time)
{
	i8251.reset(time);
}

uint8_t MSXFacMidiInterface::readIO(uint16_t port, EmuTime time)
{
	// 0 -> UART data   register
	// 1 -> UART status register
	return i8251.readIO(port & 1, time);
}

uint8_t MSXFacMidiInterface::peekIO(uint16_t port, EmuTime time) const
{
	return i8251.peekIO(port & 1, time);
}

void MSXFacMidiInterface::writeIO(uint16_t port, uint8_t value, EmuTime time)
{
	// 0 -> UART data    register
	// 1 -> UART command register
	i8251.writeIO(port & 1, value, time);
}

// I8251Interface  (pass calls from I8251 to outConnector)

void MSXFacMidiInterface::Interface::setRxRDY(bool /*status*/, EmuTime /*time*/)
{
}

void MSXFacMidiInterface::Interface::setDTR(bool /*status*/, EmuTime /*time*/)
{
}

void MSXFacMidiInterface::Interface::setRTS(bool /*status*/, EmuTime /*time*/)
{
}

bool MSXFacMidiInterface::Interface::getDSR(EmuTime /*time*/)
{
	return true;
}

bool MSXFacMidiInterface::Interface::getCTS(EmuTime /*time*/)
{
	return true;
}

void MSXFacMidiInterface::Interface::setDataBits(DataBits bits)
{
	auto& midi = OUTER(MSXFacMidiInterface, interface);
	midi.outConnector.setDataBits(bits);
}

void MSXFacMidiInterface::Interface::setStopBits(StopBits bits)
{
	auto& midi = OUTER(MSXFacMidiInterface, interface);
	midi.outConnector.setStopBits(bits);
}

void MSXFacMidiInterface::Interface::setParityBit(bool enable, Parity parity)
{
	auto& midi = OUTER(MSXFacMidiInterface, interface);
	midi.outConnector.setParityBit(enable, parity);
}

void MSXFacMidiInterface::Interface::recvByte(uint8_t value, EmuTime time)
{
	auto& midi = OUTER(MSXFacMidiInterface, interface);
	midi.outConnector.recvByte(value, time);
}

void MSXFacMidiInterface::Interface::signal(EmuTime time)
{
	const auto& midi = OUTER(MSXFacMidiInterface, interface);
	midi.getPluggedMidiInDev().signal(time);
}


// MidiInConnector

bool MSXFacMidiInterface::ready()
{
	return i8251.isRecvReady();
}

bool MSXFacMidiInterface::acceptsData()
{
	return i8251.isRecvEnabled();
}

void MSXFacMidiInterface::setDataBits(DataBits bits)
{
	i8251.setDataBits(bits);
}

void MSXFacMidiInterface::setStopBits(StopBits bits)
{
	i8251.setStopBits(bits);
}

void MSXFacMidiInterface::setParityBit(bool enable, Parity parity)
{
	i8251.setParityBit(enable, parity);
}

void MSXFacMidiInterface::recvByte(uint8_t value, EmuTime time)
{
	i8251.recvByte(value, time);
}


template<typename Archive>
void MSXFacMidiInterface::serialize(Archive& ar, unsigned /*version*/)
{
	ar.template serializeBase<MSXDevice>(*this);
	ar.template serializeBase<MidiInConnector>(*this);
	ar.serialize("outConnector", outConnector,
	             "I8251",        i8251);
}
INSTANTIATE_SERIALIZE_METHODS(MSXFacMidiInterface);
REGISTER_MSXDEVICE(MSXFacMidiInterface, "MSXFacMidiInterface");

} // namespace openmsx
