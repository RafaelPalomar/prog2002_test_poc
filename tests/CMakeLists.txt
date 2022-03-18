cmake_minimum_required (VERSION 3.8)
include (RenderdocTest.cmake)

#add_subdirectory(tests)

set(Lab1Tests renderdoc_test.cpp)


create_test_driver(Lab1 "${Lab1Tests_LIBRARIES}" "${Lab1Tests}")

add_renderdoc_test(RenderDocTest	# test name (added for ctest)
		Lab1						# exe
		renderdoc_test				# arguments to exe
		--drawcalls_count 3
)
