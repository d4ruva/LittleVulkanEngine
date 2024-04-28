#pragma once

#include"lve_window.hpp"
#include"lve_pipeline.hpp"
#include"lve_device.hpp"
#include"lve_swap_chain.hpp"

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
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();

	public:
		LveWindow lveWindow{ WIDTH, HEIGHT, "Untitled Window" };
		LveDevice lveDevice{ lveWindow};
		LveSwapChain lveSwapchain{ lveDevice, lveWindow.getExtent() };
		std::unique_ptr<LvePipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
	};
}