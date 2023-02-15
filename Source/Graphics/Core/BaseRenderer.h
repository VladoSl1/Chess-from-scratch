#pragma once

/*
BaseRenderer:
	- base class for every renderer
	- contains basic structure of every renderer (shader, vao, vbo)
*/

#include "Base.h"

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"


namespace graphics
{
	namespace core
	{
		class BaseRenderer
		{
		public:
			BaseRenderer() = default;
			BaseRenderer(Shader& shader)
				: shader(shader), specialShader(false) { }

			~BaseRenderer() {
				if (specialShader)
					shader.Delete();
				vao.Delete();
				vbo.Delete();
			}

		protected:
			Shader shader;
			VertexArray vao;
			VertexBuffer vbo;

		private:
			bool specialShader = true;
		};
	}
}
