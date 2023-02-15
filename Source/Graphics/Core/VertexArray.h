#pragma once

#include "Base.h"

#include "VertexBuffer.h"

/*
Inserts data given by VertexBuffer into specific layout in the vertex shader.
*/

namespace graphics
{
	namespace core
	{
		class VertexArray
		{
		public:
			VertexArray();
			VertexArray(int id) : _ID(id), attrib_count(0) { }

			void AddBuffer(VertexBuffer& vb, VertexBufferLayout& layout);

			void Use();
			void Unbind();
			void Delete();

			inline unsigned int ID() const { return _ID; }

		private:
			unsigned int _ID;
			unsigned int attrib_count;
		};
	}

}