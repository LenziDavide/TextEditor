#pragma once

struct GLFWwindow;

class AppGuiLayer
{
public:
	void OnAttach(GLFWwindow* window);
	void Begin();
	void End();
	void OnDetach();
};