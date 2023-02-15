#pragma once

#include "Base.h"

/*
IndexBuffer is used for vertex buffer data management.
It matches data given by VertexBuffer with it's actual positions.
Prevention against repeating coordinates (performance++).
*/


namespace graphics
{
	namespace core
	{
		class IndexBuffer
		{
		public:
			IndexBuffer() : _ID(0), _count(0) { }
			IndexBuffer(const unsigned int* data, unsigned int count)
				:_ID(0), _count(count)
			{
				GLCall(glGenBuffers(1, &_ID));
				GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ID));
				GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
			}

			/* Set as active */
			inline void Use() const {
				GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ID));
			}

			inline void Unbind() const {
				GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
			}

			inline void Delete() {
				if (_ID)
					GLCall(glDeleteBuffers(1, &_ID));
				_ID = 0;
			}

			inline unsigned int Count() const { return _count; }
			inline unsigned int ID() const { return _ID; }

		private:
			unsigned int _ID;
			unsigned int _count;
		};
	}
}
