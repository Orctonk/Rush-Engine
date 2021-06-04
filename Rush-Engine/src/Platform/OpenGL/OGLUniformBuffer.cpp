#include "OGLUniformBuffer.h"

#include <glad/glad.h>

namespace Rush {

OGLUniformBuffer::OGLUniformBuffer(uint32_t size, uint16_t binding) : UniformBuffer() {
	glGenBuffers(1, &m_UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	Bind(binding);
}

OGLUniformBuffer::~OGLUniformBuffer() {
	glDeleteBuffers(1, &m_UBO);
}

void OGLUniformBuffer::Bind(uint16_t binding) {
	glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_UBO);
}
	
void OGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset) {
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

};