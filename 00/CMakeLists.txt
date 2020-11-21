add_executable(arkanoid main.cpp)

target_include_directories(arkanoid PRIVATE ${SFML_INCLUDE_DIR})
target_compile_features(arkanoid PUBLIC cxx_std_17)
target_compile_definitions(arkanoid PRIVATE SFML_STATIC)

target_link_libraries(arkanoid ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})