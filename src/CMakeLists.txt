file(GLOB_RECURSE HTTP_SERVER_SOURCE_FILES 
		${CMAKE_CURRENT_SOURCE_DIR}/*.c)

file(GLOB_RECURSE HTTP_SERVER_HEADER_FILES 
		${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(${PROJECT_NAME} ${HTTP_SERVER_SOURCE_FILES} ${HTTP_SERVER_HEADER_FILES})

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
    target_link_libraries(${PROJECT_NAME} PRIVATE wsock32)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -pedantic)
endif()

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF
)