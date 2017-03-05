
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
    if(CLANG OR GCC)
        set_target_properties(${tname} PROPERTIES
            COMPILE_FLAGS "-Wno-builtin-macro-redefined")
    elseif(MSVC)
    else()
        target_link_libraries(${tname} zlog)
    endif()

    add_custom_target(run-${tname}
        COMMAND "${tname}"
        WORKING_DIRECTORY "${BIN_DIR}"
        DEPENDS ${tname}
    )
    add_dependencies(tests run-${tname})

    add_test(NAME run-${tname}
        COMMAND "${tname}"
        WORKING_DIRECTORY "${BIN_DIR}"
    )
endforeach(t)

target_link_libraries(test_zpath ${zpath_LIBRARIES})

