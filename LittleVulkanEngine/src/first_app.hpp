#pragma once

#include"lve_window.hpp"

namespace lve
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();
	public:
		LveWindow lveWindow{ WIDTH, HEIGHT, "Untitled WIndow" };
	};
}