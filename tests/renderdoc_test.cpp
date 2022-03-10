#include <stdio.h>
#include <array>
#include "../external/include/argparse/argparse.hpp"

std::string exec(std::string cmd) {
    std::array<char, 128> buffer{};
    std::string result;
    const std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd.c_str(), "r"), _pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int renderdoc_test(int argc, char* argv[]) {
	//argparse::ArgumentParser parser("test_framework");
	//parser.add_argument("--frame").help("frame to capture on").scan<'i', int>().default_value(0);
	//parser.add_argument("--framecount").help("amount of frames to capture").scan<'i', int>().default_value(0);
	//parser.add_argument("--test").help("run tests").default_value(false).implicit_value(true);
	//parser.add_argument("--verbose", "-v").default_value(false).implicit_value(true);
	//parser.add_argument("--log");
	printf("renderdoc_test\n");

    // TODO: hardcoded path
    std::stringstream arguments;
    for (int i = 1; i < argc; i++) {
        arguments << argv[i] << " ";
    }
    std::cout << "Arguments: " << arguments.str() << "\n";
    /* Conforms to original script
    static const std::string test_script_path = R"("G:\repos\test_framework_tester\run_tests.py")";
    static const std::string python_executable = R"("C:\Python36\python.exe)";
    static const std::string pyrenderdoc = R"(--pyrenderdoc "G:\GitProjects\renderdoc-1.18\x64\Release\pymodules")";
    static const std::string renderdoc = R"(--renderdoc "G:\GitProjects\renderdoc-1.18\x64\Release")";
    static const std::string demos_binary = R"(--demos-binary "G:\repos\test_framework\out\build\x64-Debug\test_framework.exe")";
    */

    static const std::string test_script_path = R"("G:\repos\test_framework_tester\run_tests2.py")";
    static const std::string python_executable = R"("C:\Python36\python.exe)";
    static const std::string pyrenderdoc = R"(--pyrenderdoc "G:\GitProjects\renderdoc-1.18\x64\Release\pymodules")";
    static const std::string renderdoc = R"(--renderdoc "G:\GitProjects\renderdoc-1.18\x64\Release")";
    static const std::string test_binary = R"(--test-binary "G:\repos\test_framework\out\build\x64-Debug\test_framework.exe")";

	static const std::string command =
		python_executable + " "
		+ test_script_path + " "
        + arguments.str() + " " // Pass on arguments from this executable to the test script
		+ pyrenderdoc + " "
		+ renderdoc + " "
		+ test_binary;

    std::cout << "Command: " << command << "\n";
    const auto output = exec(command);

    std::cout << "Output: " << output;
    if (output.find(R"(// fail=0)") == std::string::npos) {
        return -1;
    }

	return 0;
}
