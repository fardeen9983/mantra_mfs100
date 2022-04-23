cmake_minimum_required(VERSION 3.14)
set(PROJECT_NAME "mantra_mfs100")
project(${PROJECT_NAME} LANGUAGES CXX)

# This value is used when generating builds using this plugin, so it must
# not be changed
set(PLUGIN_NAME "mantra_mfs100_plugin")

add_library(${PLUGIN_NAME} SHARED
  "mantra_mfs100_plugin.cpp"
)
set_target_properties(${PLUGIN_NAME} PROPERTIES IMPORTED_LOCATION 
    ${CMAKE_CURRENT_SOURCE_DIR}/MFS100Dll.dll
)
set_target_properties(${PLUGIN_NAME} PROPERTIES IMPORTED_LOCATION 
    ${CMAKE_CURRENT_SOURCE_DIR}/iengine_ansi_iso.dll
)
set_target_properties(${PLUGIN_NAME} PROPERTIES IMPORTED_LOCATION 
    ${CMAKE_CURRENT_SOURCE_DIR}/WSQ_library64.dll
)

apply_standard_settings(${PLUGIN_NAME})
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${PLUGIN_NAME} PRIVATE flutter flutter_wrapper_plugin ${CMAKE_CURRENT_SOURCE_DIR}/MFS100Dll.lib ${CMAKE_CURRENT_SOURCE_DIR}/WSQ_library64.lib) # "${MFS100Dll}")

# List of absolute paths to libraries that should be bundled with the plugin
set(mantra_mfs100_bundled_libraries
  ""
  PARENT_SCOPE
)
