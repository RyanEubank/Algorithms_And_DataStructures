include(FetchContent)

FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

include_directories(${PROJECT_SOURCE_DIR}/include)

macro(package_add_test TESTNAME)
	# create an executable in which the tests will be stored
	add_executable(${TESTNAME} ${ARGN})
	# link the Google test infrastructure, mocking library, and a default main function to
	# the test executable.  Remove g_test_main if writing your own main function.
	target_link_libraries(${TESTNAME} GTest::gmock_main)
	# gtest_discover_tests replaces gtest_add_tests,
	# see https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it
	gtest_discover_tests(${TESTNAME}
		# set a working directory so your project root so that you can find test data via paths relative to the project root
		WORKING_DIRECTORY ${PROJECT_DIR}
		PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
	)
endmacro()

package_add_test(setup_test setup_test.cpp)

package_add_test(dynamic_array_collection_interface_tests dynamic_array_tests/dynamic_array_collection_interface_tests.cpp)
package_add_test(dynamic_array_iterator_tests dynamic_array_tests/dynamic_array_iterator_tests.cpp)
package_add_test(dynamic_array_access_tests dynamic_array_tests/dynamic_array_access_tests.cpp)
package_add_test(dynamic_array_allocation_tests dynamic_array_tests/dynamic_array_allocation_tests.cpp)
package_add_test(dynamic_array_emplace_tests dynamic_array_tests/dynamic_array_emplace_tests.cpp)
package_add_test(dynamic_array_insertion_tests dynamic_array_tests/dynamic_array_insertion_tests.cpp)
package_add_test(dynamic_array_removal_tests dynamic_array_tests/dynamic_array_removal_tests.cpp)

package_add_test(linked_list_collection_interface_tests linked_list_tests/linked_list_collection_interface_tests.cpp)
package_add_test(linked_list_iterator_tests linked_list_tests/linked_list_iterator_tests.cpp)
package_add_test(linked_list_access_tests linked_list_tests/linked_list_access_tests.cpp)
package_add_test(linked_list_emplace_tests linked_list_tests/linked_list_emplace_tests.cpp)
package_add_test(linked_list_insertion_tests linked_list_tests/linked_list_insertion_tests.cpp)
package_add_test(linked_list_removal_tests linked_list_tests/linked_list_removal_tests.cpp)

package_add_test(stack_collection_interface_tests stack_tests/stack_collection_interface_tests.cpp)
package_add_test(stack_interface_tests stack_tests/stack_interface_tests.cpp)

package_add_test(queue_collection_interface_tests queue_tests/queue_collection_interface_tests.cpp)
package_add_test(queue_interface_tests queue_tests/queue_interface_tests.cpp)