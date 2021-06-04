#include "Rush/core/Core.h"
#include "Rush/graphics/UniformBuffer.h"

namespace Rush {

class OGLUniformBuffer : public UniformBuffer {
private:
	uint32_t m_UBO;
public:
	OGLUniformBuffer(uint32_t size, uint16_t binding);
	virtual ~OGLUniformBuffer();

	virtual void Bind(uint16_t binding) override;
	virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
};

}