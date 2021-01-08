#include "common/base/vm.h"

#include <filesystem>
#include <iostream>
#include <fstream>

#include "vm_wrapper.hpp"
#include "execution.hpp"

struct machine_options
{
	std::string boot_path;
};

machine_options parse_arguments(const std::vector<std::string_view>& args)
{
	if(std::size(args) < 2)
	{
		throw std::runtime_error{"Usage: altair_vm path_to_binary"};
	}

	machine_options output;
	output.boot_path = args[1];

	return output;
}

std::vector<uint32_t> read_binary(const std::filesystem::path& path)
{
	std::ifstream ifs{path, std::ios_base::binary};
	if(!ifs)
	{
		throw std::runtime_error{"Can not find file \"" + path.string() + "\"."};
	}

	std::vector<uint32_t> output;
	output.resize(std::filesystem::file_size(path) / 4u);

	const auto bytes_size{static_cast<std::streamsize>(std::size(output) * 4)};
	if(ifs.read(reinterpret_cast<char*>(std::data(output)), bytes_size).gcount() != bytes_size)
	{
		throw std::runtime_error{"Can not read file \"" + path.string() + "\"."};
	}

	return output;
}

void run(const machine_options& options)
{
	const auto boot_code{read_binary(options.boot_path)};

	ar::virtual_machine machine;
	ar::processor processor{machine, std::data(boot_code), uint32_t(std::size(boot_code))};
	ar::physical_memory memory{machine};

	execute(processor);
}

int main(int argc, char** argv)
{
	try
	{
		std::vector<std::string_view> args;
		args.reserve(size_t(argc));
		for(int i = 0; i < argc; ++i)
		{
			args.emplace_back(argv[i]);
		}

		run(parse_arguments(args));
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;

		return 1;
	}
}
