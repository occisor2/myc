target_sources(myc PUBLIC
  main.cpp
  token.cpp
  scanner.cpp
  parser.cpp)

add_subdirectory(ast)
