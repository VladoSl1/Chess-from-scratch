#include "VertexArray.h"

#include "VertexBufferLayout.h"

#include <vector>

namespace graphics
{
	namespace core
	{
		VertexArray::VertexArray()
			: _ID(0), attrib_count(0)
		{
			GLCall(glGenVertexArrays(1, &_ID));
		}

		void VertexArray::Use() {
			GLCall(glBindVertexArray(_ID));
		}

		void VertexArray::Unbind() {
			GLCall(glBindVertexArray(0));
		}

		void VertexArray::Delete() {
			if (_ID)
				GLCall(glDeleteVertexArrays(1, &_ID));
			_ID = 0;
		}

		void VertexArray::AddBuffer(VertexBuffer& vb, VertexBufferLayout& layout)
		{
			Use();
			vb.Use();

			unsigned int offset = 0;
			const std::vector<VertexBufferLayoutElement> elements = layout.Elements();

			for (unsigned int i = 0; i < elements.size(); i++)
			{
				const VertexBufferLayoutElement element = elements[i];

				GLCall(glVertexAttribPointer(attrib_count + i, element.count, element.type, element.normalized, layout.Stride(), (const void*)offset));
				GLCall(glEnableVertexAttribArray(attrib_count + i));
				if (element.divisor)
					GLCall(glVertexAttribDivisor(attrib_count + i, element.divisor));

				offset += VertexBufferLayoutElement::SizeOfType(element.type) * element.count;
			}

			attrib_count += elements.size();
		}
	}

}