cmake_minimum_required(VERSION 3.14)
project(SEH)
enable_language(C CXX)
enable_testing()

aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/xcpt4/ XCPT4_SRCS)
add_executable(xcpt4 ${XCPT4_SRCS})
add_test(NAME xcpt4-test COMMAND xcpt4)

add_executable(nested_collided ${CMAKE_CURRENT_LIST_DIR}/src/nested_collided/nestcol.c)
add_test(NAME nested_collided-test COMMAND nested_collided)

add_library(xframe_eh_dll SHARED ${CMAKE_CURRENT_LIST_DIR}/src/xframe_eh_dll/xframe_eh_dll.c ${CMAKE_CURRENT_LIST_DIR}/src/xframe_eh_dll/xframe_eh_dll.DEF)
add_executable(xframe_eh_exe ${CMAKE_CURRENT_LIST_DIR}/src/xframe_eh_exe/xframe_eh_exe.c)
target_link_libraries(xframe_eh_exe PRIVATE xframe_eh_dll)
add_test(NAME xframe_eh_exe-test COMMAND xframe_eh_exe)