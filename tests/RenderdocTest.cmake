enable_testing()


include_directories(
	${CMAKE_SOURCE_DIR}/tests 
	${CMAKE_SOURCE_DIR}/tests/renderdoc
	${CMAKE_SOURCE_DIR}/external
	${CMAKE_SOURCE_DIR}/external/ppystruct/include
	${CMAKE_SOURCE_DIR}/external/renderdoc
	${CMAKE_SOURCE_DIR}/external/include
	${CMAKE_SOURCE_DIR}/external/fmt/include
)

add_compile_definitions(ENABLE_UNIT_TESTS="-")
add_compile_definitions(_RELEASE)
if (MSVC)
	add_compile_definitions(RENDERDOC_PLATFORM_WIN32 NOMINMAX)
else()
	add_compile_definitions(RENDERDOC_PLATFORM_LINUX)
endif()

# KIT = TestDriverName
# KIT_LIBS = Libraries
# KitTests = sourcefile for test(s)
function(create_test_driver KIT KIT_LIBS KitTests)
	#set(CMAKE_TESTDRIVER_BEFORE_TESTMAIN "${emscripten_before}#include \"itkTestDriverBeforeTest.inc\"")
	#set(CMAKE_TESTDRIVER_AFTER_TESTMAIN "#include \"itkTestDriverAfterTest.inc\"${emscripten_after}")

	set(ADDITIONAL_SRC ${ARGN}	
	 "renderdoc_helper.cpp" "renderdoc_helper.hpp"
	#"${CMAKE_SOURCE_DIR}/external/renderdoc/api/replay/renderdoc_replay.h"
	#"${CMAKE_SOURCE_DIR}/external/renderdoc/maths/formatpacking.h"
	#"${CMAKE_SOURCE_DIR}/external/renderdoc/maths/formatpacking.cpp"
	#"renderdoc/maths/formatpacking.h"
	##"${CMAKE_SOURCE_DIR}/external/renderdoc/api/replay/pipestate.inl"
	#"${CMAKE_SOURCE_DIR}/external/renderdoc/api/replay/renderdoc_tostr.inl"
 #"renderdoc/formatpacking.cpp"
 #"renderdoc/formatpacking.h"
 #"renderdoc/vec.hpp"
 #"renderdoc/half_convert.hpp"
	)
	#file(GLOB_RECURSE CPPFILES
     #"*.h"
     #"*.hpp"
     #"*.cpp"
	#)
	#message(WARNING "src: ${CPPFILES}")

	create_test_sourcelist(Tests ${KIT}TestDriver.cxx ${KitTests})

	add_executable(${KIT}TestDriver ${KIT}TestDriver.cxx ${Tests} ${ADDITIONAL_SRC} "float16.h" "float16.cpp" "formatpacking.h" "formatpacking.cpp" "vec.h")

	# FIXME
	target_link_libraries(${KIT}TestDriver LINK_PUBLIC ${KIT_LIBS} "G:/repos/renderdoc/x64/Release/renderdoc.lib" fmt CPPYSTRUCT)


	target_compile_features(${KIT}TestDriver PRIVATE cxx_std_17)
endfunction()

function(add_renderdoc_test TEST_NAME KIT)
	add_test(${TEST_NAME} ${KIT}TestDriver ${ARGN})
endfunction()


function (add_python_renderdoc_test TEST_NAME TEST_DRIVER_ARGS COMMAND)

endfunction()
