#pragma once

#include"lve_window.hpp"
#include"lve_pipeline.hpp"
#include"lve_device.hpp"
#include"lve_swap_chain.hpp"
#include"lve_model.hpp"

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
		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void recreateSwapchains();
		void recordCommandBuffer(int imageIndex);

		// added hw
		void sierpinski(
			std::vector<LveModel::Vertex>& vertices,
			int depth,
			glm::vec2 left,
			glm::vec2 right,
			glm::vec2 top);

	public:
		LveWindow lveWindow{ WIDTH, HEIGHT, "Untitled Window" };
		LveDevice lveDevice{ lveWindow};
		std::unique_ptr<LveSwapChain> lveSwapchain;
		std::unique_ptr<LvePipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::unique_ptr<LveModel> lveModel;
	};
}