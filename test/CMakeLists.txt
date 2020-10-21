file(GLOB_RECURSE C_SOURCES ${CMAKE_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE ASM_SOURCES ${CMAKE_SOURCE_DIR}/src/*.s)

add_mocked_test(main SOURCES ${C_SOURCES} ${ASM_SOURCES} MOCKS printf scanf fscanf fprintf main __isoc99_scanf)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

