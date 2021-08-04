cmake_minimum_required(VERSION 3.10)

project(webUi VERSION 0.0.1)

file(GLOB pyFiles ${CMAKE_CURRENT_SOURCE_DIR}/*.py)

file(GLOB staticFiles ${CMAKE_CURRENT_SOURCE_DIR}/static/*
)

file(GLOB templatesFiles ${CMAKE_CURRENT_SOURCE_DIR}/templates/*.html)

add_custom_target(deploy-webui ALL
    DEPENDS deploy-files)

add_custom_command(OUTPUT deploy-files
    COMMAND ${CMAKE_COMMAND} -E make_directory  ${CMAKE_BINARY_DIR}/www
    COMMAND ${CMAKE_COMMAND} -E copy ${pyFiles} ${CMAKE_BINARY_DIR}/www

    COMMAND ${CMAKE_COMMAND} -E make_directory  ${CMAKE_BINARY_DIR}/www/static
    COMMAND ${CMAKE_COMMAND} -E copy ${staticFiles} ${CMAKE_BINARY_DIR}/www/static

    COMMAND ${CMAKE_COMMAND} -E make_directory  ${CMAKE_BINARY_DIR}/www/templates
    COMMAND ${CMAKE_COMMAND} -E copy ${templatesFiles} ${CMAKE_BINARY_DIR}/www/templates
)

add_custom_command(TARGET deploy-webui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_BINARY_DIR}/webui
)
