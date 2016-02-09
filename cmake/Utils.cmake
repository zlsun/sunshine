
macro(get_basename basename file)
    string(REGEX REPLACE ".*/([^.]*)\\..*" "\\1" ${basename} ${file})
endmacro(get_basename)

