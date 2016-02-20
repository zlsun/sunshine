
macro(get_basename basename file)
    string(REGEX REPLACE ".*/([^.]*)\\..*" "\\1" ${basename} ${file})
endmacro(get_basename)

macro(pre_build TARGET_NAME)
    add_custom_target(${TARGET_NAME}_PRE_BUILD ALL)
    add_custom_command(
        TARGET ${TARGET_NAME}_PRE_BUILD
        ${ARGN}
        PRE_BUILD
        COMMENT "${TARGET_NAME}_PRE_BUILD ..."
    )
    add_custom_target(${TARGET_NAME}_CORE_PRE_BUILD)
    add_dependencies(${TARGET_NAME}_PRE_BUILD ${TARGET_NAME}_CORE_PRE_BUILD)
    add_dependencies(${TARGET_NAME} ${TARGET_NAME}_PRE_BUILD)
endmacro()

