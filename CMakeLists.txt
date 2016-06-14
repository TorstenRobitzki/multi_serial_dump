cmake_minimum_required(VERSION 2.8.12)

#set(Boost_DEBUG 1)
find_package( Boost REQUIRED COMPONENTS program_options system )
if (Boost_FOUND)
    include_directories(BEFORE ${Boost_INCLUDE_DIR})
endif()

add_compile_options(-std=c++11 -Wall -pedantic -Wno-logical-op-parentheses)

add_executable(multi_serial_dump multi_serial_dump.cpp interface.cpp)
target_link_libraries(multi_serial_dump ${Boost_LIBRARIES})

add_executable(interface_tests interface_tests.cpp interface.cpp)
target_link_libraries(interface_tests ${Boost_LIBRARIES})
