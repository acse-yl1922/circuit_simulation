project(utils)

list(APPEND Utils AlgorithmEvaluator)

foreach(UTIL IN LISTS Utils)
    add_executable(${UTIL} ${UTIL}.cpp)
    target_link_libraries(${UTIL} geneticAlgorithm circuitSimulator)
    target_include_directories(${UTIL} PRIVATE ../include)
    set_target_properties(${UTIL} PROPERTIES
        CXX_STANDARD 17
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/utils/bin")
endforeach()
