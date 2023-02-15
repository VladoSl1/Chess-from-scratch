#pragma once

#include "Base.h"

/*
Container used for sending data to graphic card
*/

namespace graphics
{
	namespace core
	{
		class VertexBuffer
		{
		public:
			VertexBuffer() : _ID(0) { }
			/* size: allocated space in bytes */
			VertexBuffer(unsigned int size);
			/* size: size of data in bytes */
			VertexBuffer(const void* data, unsigned int size);

			void Use();
			void Unbind();
			void Delete();

			void BufferSubData(const void* data, unsigned int size, unsigned int offset = 0);

			inline unsigned int ID() const { return _ID; }

		private:
			unsigned int _ID;
		};
	}
}