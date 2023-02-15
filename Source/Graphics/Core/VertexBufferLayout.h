#pragma once

#include "Base.h"

#include <iostream>
#include <vector>

namespace graphics
{
	namespace core
	{
		struct VertexBufferLayoutElement
		{
			unsigned int type;
			unsigned int count;
			unsigned int normalized;
			unsigned int divisor = 0;

			static unsigned int SizeOfType(unsigned int type)
			{
				switch (type)
				{
				case GL_FLOAT: return 4;
				default: std::cout << "Wrong Type\n"; return 0;
				}
			}
		};

		/*
		Data given by VertexBuffer are distributed into corresponding layouts
		in vertex shader by VertexArray. VertexArray uses VertexBufferLayout to
		know where are given data meant to be.
		*/

		class VertexBufferLayout
		{

		public:
			VertexBufferLayout() : _stride(0) { }
			~VertexBufferLayout() { };

			template<typename Type>
			void Add(unsigned int count, unsigned int divisor = 0)
			{
				throw std::invalid_argument("Adding currently unsuported type!");
			}

			template<>
			void Add<float>(unsigned int count, unsigned int divisor)
			{
				_elements.push_back({ GL_FLOAT, count, GL_TRUE, divisor });
				_stride += VertexBufferLayoutElement::SizeOfType(GL_FLOAT) * count;
			}

			inline unsigned int Stride() const { return _stride; }
			inline std::vector<VertexBufferLayoutElement> Elements() const { return _elements; }

		private:
			std::vector<VertexBufferLayoutElement> _elements;
			unsigned int _stride;
		};
	}
}