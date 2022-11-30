find_package(Qt6 COMPONENTS Widgets WebEngineWidgets REQUIRED)

add_executable(${PROJECT_NAME}
               Main.cxx)

if(WIN32)
set_target_properties(${PROJECT_NAME}
                      PROPERTIES WIN32_EXECUTABLE TRUE)
endif(WIN32)

target_link_libraries(${PROJECT_NAME}
                      PRIVATE Qt6::Widgets Qt6::WebEngineWidgets)
