#include"lve_renderer.hpp"

#include <array>
#include <stdexcept>
#include <cassert>



namespace lve
{



	LveRenderer::LveRenderer(LveWindow& window, LveDevice &device) : lveWindow{window}, lveDevice{device}
	{
		recreateSwapchains();
		createCommandBuffers();
	}

	LveRenderer::~LveRenderer()
	{
		freeCommandBuffers();
	}


	void LveRenderer::recreateSwapchains()
	{
		auto extent = lveWindow.getExtent();

		while (extent.width == 0 || extent.height == 0)
		{
			extent = lveWindow.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(lveDevice.device());

		if (lveSwapchain == nullptr)
		{
			lveSwapchain = std::make_unique<LveSwapChain>(lveDevice, extent);
		}
		else
		{
			std::shared_ptr<LveSwapChain> oldSwapChain = std::move(lveSwapchain);
			lveSwapchain = std::make_unique<LveSwapChain>(lveDevice, extent, oldSwapChain);

			if (!oldSwapChain->comapreSwapFormat(*lveSwapchain.get()))
			{
				throw std::runtime_error("Swapchain image or  depth format has changed!!");
			}
		}
	}

	void LveRenderer::createCommandBuffers()
	{
		commandBuffers.resize(LveSwapChain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = lveDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(lveDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create command buffers");
		}
	}

	void LveRenderer::freeCommandBuffers()
	{
		vkFreeCommandBuffers(lveDevice.device(), lveDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
		commandBuffers.clear();
	}


	VkCommandBuffer LveRenderer::beginFrame()
	{
		assert(!isFrameStarted && "Cant clall beginFrame while already in progrss");

		auto result = lveSwapchain->acquireNextImage(&currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapchains();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Failed to acquire image from swapchain");
		}


		isFrameStarted = true;

		auto commandBuffer = getCurrentCommandBuffer();

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to begin command buffers");
		}

		return commandBuffer;
	}

	void LveRenderer::endFrame()
	{
		assert(isFrameStarted && "Cant't call endframe when frame is started");
		auto commandBuffer = getCurrentCommandBuffer();

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to record command buffers");
		}

		auto result = lveSwapchain->submitCommandBuffers(&commandBuffer, &currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || lveWindow.wasWindowResized())
		{
			lveWindow.resetWindowResizedFlag();
			recreateSwapchains();
		}
		else if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to cubmit commands:");
		}

		isFrameStarted = false;
		currentFrameIndex = (currentFrameIndex + 1) % LveSwapChain::MAX_FRAMES_IN_FLIGHT;
	}

	void LveRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "Cant call beginSwapchain if frame is not in progrss");
		assert(commandBuffer == getCurrentCommandBuffer() && "Cant beegin render pass on command buffer froma a different frame");


		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = lveSwapchain->getRenderPass();
		renderPassInfo.framebuffer = lveSwapchain->getFrameBuffer(currentImageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = lveSwapchain->getSwapChainExtent();

		std::array<VkClearValue, 2>clearValues;
		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(lveSwapchain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(lveSwapchain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, lveSwapchain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	}

	void LveRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "Cant call endSwapchain if frame is not in progrss");
		assert(commandBuffer == getCurrentCommandBuffer() && "Cant beegin render pass on command buffer froma a different frame");

		vkCmdEndRenderPass(commandBuffer);

	}
}