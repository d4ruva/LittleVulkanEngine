#include"first_app.hpp"

#include<iostream>

int main()
{
	lve::FirstApp app{};

	try
	{
		app.run();
	}
	catch (const std::exception e)
	{
		std::cerr << e.what() << std::endl; 
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}