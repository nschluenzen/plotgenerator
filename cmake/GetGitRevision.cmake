macro(GetGitRevision)

find_package(Git)

execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
                OUTPUT_VARIABLE GIT_REVISION
                RESULT_VARIABLE GIT_REVISION_RESULT
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                ERROR_QUIET
                OUTPUT_STRIP_TRAILING_WHITESPACE)

if(NOT ${GIT_REVISION_RESULT} EQUAL 0)
   set(GIT_REVISION "unknown")
endif(NOT ${GIT_REVISION_RESULT} EQUAL 0) 

message(STATUS "current Git revision is: ${GIT_REVISION}")

endmacro(GetGitRevision)
