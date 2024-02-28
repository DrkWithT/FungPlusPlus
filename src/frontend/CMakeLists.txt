# src/frontend CMakeLists

add_library(frontend "")

target_sources(frontend PRIVATE token.cpp lexer.cpp parser.cpp)
