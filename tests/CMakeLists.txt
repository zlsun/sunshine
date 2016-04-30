
# touch testdir/file
set(TEST_FILE "${BIN_DIR}/testdir/file")
file(WRITE "${TEST_FILE}" "")

file(GLOB tests "*.cpp")

add_custom_target(tests)

foreach(t ${tests})
    get_basename(basename ${t})
    string(CONCAT tname test_ ${basename})

    add_executable(${tname} ${t})
    target_compile_definitions(${tname}
        PRIVATE "__FILE__=\"${basename}\"")
    if(CMAKE_COMPILER_IS_GNUCXX)
        set_target_properties(${tname} PROPERTIES
            COMPILE_FLAGS "-Wno-builtin-macro-redefined")
    endif()
    target_link_libraries(${tname} zlog)
    if(WINDOWS)
        set(exe "${tname}")
    else()
        set(exe "./${tname}")
    endif()
    add_custom_target(run_${tname}
        COMMAND "${exe}"
        WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
        DEPENDS ${tname}
    )
    add_dependencies(tests run_${tname})
endforeach(t)

target_link_libraries(test_zpath zpath)

