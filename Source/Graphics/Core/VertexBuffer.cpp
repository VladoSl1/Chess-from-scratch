#include "VertexBuffer.h"

namespace graphics
{
	namespace core
	{
		VertexBuffer::VertexBuffer(const void* data, unsigned int size)
			: _ID(0)
		{
			GLCall(glGenBuffers(1, &_ID));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, _ID));
			GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
		}

		VertexBuffer::VertexBuffer(unsigned int size)
			: _ID(0)
		{
			GLCall(glGenBuffers(1, &_ID));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, _ID));
			GLCall(glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW));
		}

		void VertexBuffer::BufferSubData(const void* data, unsigned int size, unsigned int offset) {
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
		}

		void VertexBuffer::Use() {
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, _ID));
		}

		void VertexBuffer::Unbind() {
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, NULL));
		}

		void VertexBuffer::Delete() {
			if (_ID)
				GLCall(glDeleteBuffers(1, &_ID));
			_ID = 0;
		}
	}
}