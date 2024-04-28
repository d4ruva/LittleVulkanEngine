#include"lve_pipeline.hpp"

#include<fstream>
#include<stdexcept>
#include<iostream>

namespace lve
{
	LvePipeline::LvePipeline(const std::string& vertFilePath, const std::string& fragFilePath)
	{
		createGraphicsPipeline(vertFilePath, fragFilePath);
	}


	std::vector<char> LvePipeline::readFile(const std::string& filePath)
	{
		std::ifstream file(filePath, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			throw std::runtime_error("Failed to Open Shader File: " + filePath);
		}

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;

	}

	void LvePipeline::createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath)
	{
		auto vertCode = readFile(vertFilePath);
		auto fragCode = readFile(fragFilePath);

		std::cout << "Vertex Shader Size: " << vertCode.size() << std::endl;
		std::cout << "Fragment Shader Size: " << fragCode.size() << std::endl;
	}
}