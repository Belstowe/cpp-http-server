file(GLOB_RECURSE HTTP_SERVER_SOURCE_FILES 
		${CMAKE_CURRENT_SOURCE_DIR}/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

file(GLOB_RECURSE HTTP_SERVER_HEADER_FILES 
		${CMAKE_CURRENT_SOURCE_DIR}/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

add_executable(${PROJECT_NAME} ${HTTP_SERVER_SOURCE_FILES} ${HTTP_SERVER_HEADER_FILES})

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    target_link_libraries(${PROJECT_NAME} PRIVATE wsock32)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -pedantic)
endif()

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)