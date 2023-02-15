#pragma once

#include "Core/Base.h"

#include "Mathematics.h"
#include "Mathematics/vec4.h"



namespace graphics
{
	class Window
	{
	public:
		size_t Width, Height;

		Window(size_t width, size_t height, const char* name, bool VSync = true);
		~Window();

		/* Calls PollEvents() and SwapBuffers() */
		void Update();

		/* Swap front and back buffers */
		void SwapBuffers();

		/* FPS counter */
		void ShowFPS();

		/* Poll for and process events */
		void PollEvents();

		/* Clear screen with given color (DEFAULT: vec4(0.0f, 0.0f, 0.0f, 0.0f)) */
		void ClearScreen(math::vec4<> color = math::vec4(0.0f, 0.0f, 0.0f, 0.0f));

		/* Close window */
		void CloseWindow();

		inline GLFWwindow* GetWindow() const { return window; }
		inline int WindowShouldClose() const { return glfwWindowShouldClose(window); }

	private:
		GLFWwindow* window;
		const char* window_name;

		/* FPS counter variables */
		double lastTime = 0.0L;
		unsigned int FrameNumber = 0;

		/* Resize window */
		static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	};
}

