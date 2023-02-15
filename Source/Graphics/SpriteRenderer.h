#pragma once

#include "Core/BaseRenderer.h"
#include "Core/IndexBuffer.h"

#include "Texture2D.h"

#include "Mathematics/vec2.h"
#include "Mathematics/vec3.h"
#include "Mathematics/mat4.h"

#include <vector>

#define uint	unsigned int
#define uchar	unsigned char

namespace graphics
{
	class GameObject;

	struct GraphicQueue
	{
		Texture2D texture;
		math::vec3<> color;
		math::mat4<> model;
		std::vector<math::vec2<>> offsets;

		inline unsigned int size() const { return offsets.size(); }
		inline void AddOffset(math::vec2<> model) { offsets.push_back(model); }
	};

	class SpriteRenderer : protected core::BaseRenderer
	{
		const uint SubDataPackSize = 1000U;

	public:
		SpriteRenderer(size_t width, size_t height, bool instance_option = false);
		~SpriteRenderer();

		void DrawSprite(Texture2D& texture, math::vec2<> position,
			math::vec2<> size = math::vec2(15.0f, 15.0f), float rotation = 0.0f,
			math::vec3<> color = math::vec3(1.0f));

		void CreateQueue(math::mat4<> model, Texture2D& texture, uint* queueID, math::vec3<> color = math::vec3(1.0f));
		void CreateQueue(GameObject& baseObject, uint* queueID);

		inline void DeleteQueue(uint ID) {
			queues.erase(queues.begin() + ID - 1);
		};

		inline void ResetQueue(uint ID) {
			queues[ID - 1].offsets.clear();
		};

		inline void AddToQueue(uint ID, math::vec2<> position) {
			queues[ID - 1].AddOffset(position);
		}

		void DrawQueue(uint queueID);

	protected:
		core::IndexBuffer ibo;
		core::VertexBuffer instance_vbo;

		void InitData(bool instance_option);
		void InitShader(bool instance_option);

	private:
		std::vector<GraphicQueue> queues;
	};
}
