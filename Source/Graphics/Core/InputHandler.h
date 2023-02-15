#include "Base.h"
#include "ErrorHandler.h"

#include <unordered_map>

#include "Timer.h"


namespace graphics
{
	enum class ButtonStates : unsigned char
	{
		Press			= 0b010,
		Release			= 0b001,
		ImmediateChange = 0b100,
		NoChange		= 0b000,
		NoAction		= 0b000
	};

	struct KeyData
	{
		unsigned char state;
		double time;
	};


	namespace core
	{
		class InputHandler
		{
		protected:
			using uint = unsigned int;
			using uchar = unsigned char;

			uchar size;
			const uchar capacity;

			bool* pressed;
			Timer* timers;
			std::unordered_map<uint, uint> keyMap;

			GLFWwindow* window;

		public:

			InputHandler(GLFWwindow* window, uchar numberOfKeys)
				: size(0), window(window), capacity(numberOfKeys)
			{
				pressed = new bool[numberOfKeys];
				timers = new Timer[numberOfKeys];
			}

			~InputHandler()
			{
				delete[] pressed;
				delete[] timers;
			}

			void BindKey(uchar keyID)
			{
				ASSERT(size < capacity);

				timers[size].reset();
				keyMap[keyID] = size++;
			}

			virtual KeyData GetButtonState(uchar keyID) { return { 0, 0.0 }; }

		protected:
			uchar UpdateKeyState(uint buttonID, bool press)
			{
				uchar state;

				if (press)
				{
					state = (uchar)ButtonStates::Press;

					if (!pressed[buttonID])
					{
						timers[buttonID].reset();
						pressed[buttonID] = true;
						return state | (uchar)ButtonStates::ImmediateChange;
					}
				}
				else
				{
					state = (uchar)ButtonStates::Release;

					if (pressed[buttonID])
					{
						timers[buttonID].reset();
						pressed[buttonID] = false;
						return state | (uchar)ButtonStates::ImmediateChange;
					}
				}
				return state;
			}
		};
	}	
}