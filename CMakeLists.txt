set(the_description "The Core Functionality")
ut_add_module(utcore ${TINYXML_LIBRARIES} ${LOG4CPP_LIBRARIES} ${LAPACK_LIBRARIES} ${Boost_LIBRARIES})
ut_module_include_directories(${UBITRACK_CORE_DEPS_INCLUDE_DIR})
ut_glob_module_sources(HEADERS "src/*.h" "src/*/*.h" "src/*/*/*.h" "src/*/*/*/*.h" "src/*/*/*/*/*.h" SOURCES "src/*/*.cpp" "src/*/*/*.cpp" "src/*/*/*/*.cpp" "src/*/*/*/*/*.cpp")
ut_create_module()

ut_add_module_tests()