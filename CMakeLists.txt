project(Chess)

find_package(SFML REQUIRED network audio graphics window system)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/res" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")

add_executable(chess src/main.cpp src/piece.cpp)

target_link_libraries(chess PRIVATE sfml-system sfml-window sfml-graphics sfml-network sfml-audio)