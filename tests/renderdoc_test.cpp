#include <argparse/argparse.hpp>
#include <fmt/core.h>
#include "renderdoc_helper.hpp"



REPLAY_PROGRAM_MARKER();

bool parse_arguments(argparse::ArgumentParser& parser, int argc, char* argv[]) {
	parser.add_argument("--drawcalls_count").help("target drawcalls count").scan<'i', int>().default_value(-1);

	try {
		parser.parse_args(argc, argv);
	}
	catch (const std::runtime_error& err) {
		std::cerr << err.what() << std::endl;
		std::cerr << parser;
		return false;
	}
	return true;
}

int renderdoc_test(int argc, char* argv[]) {

	//RenderDocHelper helper(R"(G:\repos\test_framework_tester\tmp\TestFramework\capture_frame5.rdc)");
	//helper.open_capture();
	//helper.dump_actions();
	//return 0;
	argparse::ArgumentParser parser("test_framework");
	if (!parse_arguments(parser, argc, argv))
		return -1;

	const auto expected_drawcalls_count = parser.get<int>("drawcalls_count");

	RenderDocHelper helper(R"(G:\repos\test_framework_tester\tmp\TestFramework\capture_frame5.rdc)");
	helper.open_capture();
	helper.dump_actions();

	if (expected_drawcalls_count != -1) {
		if (const auto drawcalls_count = helper.drawcalls_count();
			static_cast<size_t>(expected_drawcalls_count) != drawcalls_count) {
			fmt::print("found {} drawcalls, expected {}", drawcalls_count, expected_drawcalls_count);
			return -1;
		}
	}

	return 0;
}
