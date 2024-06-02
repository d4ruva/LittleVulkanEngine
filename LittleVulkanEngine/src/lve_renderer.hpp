#pragma once

#include"lve_window.hpp"
#include"lve_device.hpp"
#include"lve_swap_chain.hpp"


#include <memory>
#include <vector>
#include <cassert>

namespace lve
{
	class LveRenderer
	{
	public:

		LveRenderer(LveWindow& window, LveDevice& device);
		~LveRenderer();


		bool isFrameInProgress() const { return isFrameStarted; }

		VkRenderPass getSwapchainRenderPass() const { return lveSwapchain->getRenderPass(); }
		float getAspectRatio() const { return lveSwapchain->extentAspectRatio(); }
		VkCommandBuffer getCurrentCommandBuffer() const { assert(isFrameStarted&& "Cannot get command buffer when frame not in progress"); return commandBuffers[currentFrameIndex]; }

		int getFrameIndex() const { assert(isFrameStarted && "Cannot get frame index when frame not in progrss"); return currentFrameIndex; }
		
		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapchains();

		LveWindow& lveWindow;
		LveDevice& lveDevice;
		std::unique_ptr<LveSwapChain> lveSwapchain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex;

		bool isFrameStarted;
	};
}