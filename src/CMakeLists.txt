include(GetGitRevision)
GetGitRevision()
configure_file ("${PROJECT_SOURCE_DIR}/src/version.hpp.in"
                "${PROJECT_SOURCE_DIR}/src/version.hpp")
add_executable(plotgenerator sort.cpp)
# target_link_libraries(plotgenerator ${Boost_LIBRARIES})
set(target_list "plotgenerator" ${target_list})
install(TARGETS plotgenerator DESTINATION .)
