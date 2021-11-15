
add_subdirectory(lib/googletest)

include_directories(googletest/googletest/include googletest)

add_executable(dxfrw_test ConverterTest.cpp)

target_include_directories(dxfrw_test PRIVATE src lib/googletest/googletest/include ../src)
target_link_libraries(dxfrw_test gtest gtest_main dxfrw)

add_custom_command(
        TARGET dxfrw_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/tests/data
        ${CMAKE_BINARY_DIR}/tests/data
)
#install(TARGETS dxfrw_test
#        DESTINATION ${CMAKE_BINARY_DIR}/docker/test/
#        )


add_test(NAME dxfrw_test COMMAND $dxfrw_test)

