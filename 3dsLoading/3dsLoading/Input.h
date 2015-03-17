#pragma once

#include <OpenGL\glew.h>
#include <OpenGL\glfw3.h>
#include <OpenGL\GLM\glm.hpp>
#include <OpenGL\GLM\gtx\transform.hpp>
#include <cstdio>
#include <cstdlib>

enum KeyCodes
{
	A = GLFW_KEY_A,
	B, 
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	Num0 = GLFW_KEY_0,
	Num1,
	Num2,
	Num3,
	Num4,
	Num5,
	Num6,
	Num7,
	Num8,
	Num9,
	LeftArrow = GLFW_KEY_LEFT,
	RightArrow = GLFW_KEY_RIGHT,
	UpArrow = GLFW_KEY_UP,
	DownArrow = GLFW_KEY_DOWN,
	LeftShift = GLFW_KEY_LEFT_SHIFT,
	RightShift = GLFW_KEY_RIGHT_SHIFT,
	LeftAlt = GLFW_KEY_LEFT_ALT,
	RightAlt = GLFW_KEY_RIGHT_ALT,
	LeftControl = GLFW_KEY_LEFT_CONTROL,
	RightControl = GLFW_KEY_RIGHT_CONTROL,
	Space = GLFW_KEY_SPACE,
	Tab = GLFW_KEY_TAB,
	Backspace = GLFW_KEY_BACKSPACE,
	Slash = GLFW_KEY_SLASH,
	Backslash = GLFW_KEY_BACKSLASH,
	F1 = GLFW_KEY_F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	Delete = GLFW_KEY_DELETE,
	Home = GLFW_KEY_HOME,
	Insert = GLFW_KEY_INSERT,
	PageUp = GLFW_KEY_PAGE_UP,
	PageDown = GLFW_KEY_PAGE_DOWN,
	End = GLFW_KEY_END,
	Enter = GLFW_KEY_ENTER,
	Apostrophe = GLFW_KEY_APOSTROPHE,
	CapsLock = GLFW_KEY_CAPS_LOCK,
	Comma = GLFW_KEY_COMMA,
	Semicolon = GLFW_KEY_SEMICOLON,
	Escape = GLFW_KEY_ESCAPE
};

enum MouseButtons
{
	Left = GLFW_MOUSE_BUTTON_LEFT,
	Middle = GLFW_MOUSE_BUTTON_MIDDLE,
	Right = GLFW_MOUSE_BUTTON_RIGHT
};

class Input
{
private:
	double m_mouseX;
	double m_mouseY;

	GLFWwindow* m_window;

public:
	Input();
	~Input();

	bool Initialize(GLFWwindow* window);
	void Shutdown();

	glm::vec2 GetMousePosition();

	bool IsKeyDown(KeyCodes keyCode);
	bool IsKeyUp(KeyCodes keyCode);
	bool IsMouseButtonDown(MouseButtons button);
	bool IsMouseButtonUp(MouseButtons button);
};

