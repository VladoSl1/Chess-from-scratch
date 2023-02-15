#pragma once

#include "Core/InputHandler.h"

#include "Mathematics/vec2.h"

namespace graphics
{
	class Keyboard : public core::InputHandler
	{
	public:
		Keyboard(GLFWwindow* window, uchar numberOfKeys)
			: InputHandler(window, numberOfKeys) { }


		KeyData GetButtonState(uchar keyID) override
		{
			ASSERT(keyMap.find(keyID) != keyMap.end());

			uchar buttonID = keyMap[keyID];
			uchar state = UpdateKeyState(buttonID, glfwGetKey(window, keyID) == GLFW_RELEASE);

			return { state, timers[buttonID].deltaTime<>() };
		}
	};


	class Mouse : public core::InputHandler
	{
		/*
		Mouse position is stored in Mouse::x, Mouse::y
		*/

	public:
		inline static math::vec2<double> pos;

		Mouse(GLFWwindow* window, uchar numberOfKeys)
			: InputHandler(window, numberOfKeys)
		{
			pos = { -1.0, -1.0 };
			glfwSetCursorPosCallback(window, mouse_callback);
		}

		KeyData GetButtonState(uchar keyID) override
		{
			ASSERT(keyMap.find(keyID) != keyMap.end());

			uchar buttonID = keyMap[keyID];
			uchar state = UpdateKeyState(buttonID, glfwGetMouseButton(window, keyID));

			return { state, timers[buttonID].deltaTime<>() };
		}

	private:

		static inline void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
			pos = { xpos, ypos };
		}
	};
}