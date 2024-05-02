#pragma once

#include"lve_window.hpp"
#include"lve_device.hpp"
#include"lve_game_object.hpp"
#include"lve_renderer.hpp"

#include<memory>
#include<vector>

namespace lve
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();

		void run();

	private:
		void loadGameObjects();


	public:
		LveWindow lveWindow{ WIDTH, HEIGHT, "Untitled Window" };
		LveDevice lveDevice{ lveWindow};
		LveRenderer lveRenderer{ lveWindow, lveDevice };
		
		std::vector<LveGameObject> gameObjects;
	};
}