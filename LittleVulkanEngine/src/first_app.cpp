#include"first_app.hpp"

#include"simple_render_system.hpp"

#include <array>
#include <stdexcept>
#include <cassert>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include<glm/gtc/constants.hpp>


namespace lve
{
	FirstApp::FirstApp()
	{
		loadGameObjects();
	}

	FirstApp::~FirstApp()
	{
	}

	void FirstApp::run()
	{
		SimpleRenderSystem simpleRenderSystem{ lveDevice, lveRenderer.getSwapchainRenderPass() };
		while (!lveWindow.shouldClose())
		{
			glfwPollEvents();

			if (auto commandBuffer = lveRenderer.beginFrame())
			{
				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lveDevice.device());
	}

	void FirstApp::loadGameObjects()
	{
#if 1
		std::vector<LveModel::Vertex> vertices{
			{{ 0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f }},
			{{ 0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f }},
			{{-0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f }}
		};
#elif 0
		std::vector<LveModel::Vertex> vertices;
		sierpinski(vertices, 5, { -0.5f, 0.5f }, { 0.5f, 0.5f, }, { 0.0f, -0.5f});
#endif
		auto lveModel = std::make_shared<LveModel>(lveDevice, vertices);

		auto triangle = LveGameObject::createGameObject();
		triangle.model = lveModel;
		triangle.color = { .8f, .1f, .1f };
		triangle.transform2d.translation.x = .2f;
		triangle.transform2d.scale = { 2.f, .5f };
		triangle.transform2d.rotation = .25f * glm::two_pi<float>();

		gameObjects.push_back(std::move(triangle));

	}
}