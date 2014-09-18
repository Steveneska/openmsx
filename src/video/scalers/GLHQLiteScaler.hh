#ifndef GLHQLITESCALER_HH
#define GLHQLITESCALER_HH

#include "GLScaler.hh"
#include "GLUtil.hh"
#include "noncopyable.hh"
#include <cstdint>

namespace openmsx {

class GLHQLiteScaler final : public GLScaler, private noncopyable
{
public:
	explicit GLHQLiteScaler(GLScaler& fallback);

	virtual void scaleImage(
		gl::ColorTexture& src, gl::ColorTexture* superImpose,
		unsigned srcStartY, unsigned srcEndY, unsigned srcWidth,
		unsigned dstStartY, unsigned dstEndY, unsigned dstWidth,
		unsigned logSrcHeight);
	virtual void uploadBlock(
		unsigned srcStartY, unsigned srcEndY,
		unsigned lineWidth, FrameSource& paintFrame);

private:
	GLScaler& fallback;
	gl::Texture edgeTexture;
	gl::Texture offsetTexture[3];
	gl::PixelBuffer<uint16_t> edgeBuffer;
};

} // namespace openmsx

#endif
