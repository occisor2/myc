target_sources(myc PUBLIC
  main.cpp
  token.cpp
  scanner.cpp
  ast.cpp
  parser.cpp
  diagnostic.cpp)

add_subdirectory(analysis)
