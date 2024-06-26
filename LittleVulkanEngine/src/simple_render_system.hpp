#pragma once

#include"lve_camera.hpp"
#include"lve_pipeline.hpp"
#include"lve_device.hpp"
#include"lve_game_object.hpp"

#include<memory>
#include<vector>

namespace lve
{
	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(LveDevice& device, VkRenderPass renderPass);
		~SimpleRenderSystem();

		void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<LveGameObject>& gameObjects, const LveCamera& camera);

	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

	public:
		LveDevice &lveDevice;
		std::unique_ptr<LvePipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;
	};
}