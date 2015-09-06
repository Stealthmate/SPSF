set(TEST_LIST "")

add_executable(Main ${CURDIR}/Main.cpp)
add_dependencies(Main SPSF)
target_link_libraries(Main SPSF)
list(APPEND TEST_LIST Window)