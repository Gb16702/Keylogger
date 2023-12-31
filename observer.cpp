#include <string>
#include <iostream>
#include <fstream>

std::streampos cron_task(const std::string& file_name) {
	std::ifstream file(file_name, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Error opening file: " << file_name << std::endl;
		return -1;
	}

	file.seekg(0, std::ios::end);

	std::streampos file_size = file.tellg();

	file.close();

	return file_size;
}
