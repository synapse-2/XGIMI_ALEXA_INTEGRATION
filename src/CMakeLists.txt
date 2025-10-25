# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.*)

# for some reason the include files are not put in the right path ------- start 
#FILE(GLOB_RECURSE app_sources2 ${CMAKE_SOURCE_DIR}/managed_components/*.h)
#list (APPEND app_sources ${app_sources2})
# for some reason the include files are not put in the right path -------- end

idf_component_register(SRCS ${app_sources})



