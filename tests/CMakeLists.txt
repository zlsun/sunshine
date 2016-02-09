
# touch testdir/file
set(TEST_FILE "${BIN_DIR}/testdir/file")
file(WRITE "${TEST_FILE}" "")

file(GLOB tests "*.cpp")

add_custom_target(tests)

foreach(t ${tests})
    get_basename(tname ${t})
    string(CONCAT tname test_ ${tname})
    add_executable(${tname} ${t})
    set_target_properties(${tname} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}"
    )
    add_custom_target(run_${tname}
        COMMAND "./${tname}"
        WORKING_DIRECTORY "${BIN_DIR}"
        DEPENDS ${tname}
    )
    add_dependencies(tests run_${tname})
endforeach(t)

target_link_libraries(test_zpath zpath)

