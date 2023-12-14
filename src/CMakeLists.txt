target_sources(myc PRIVATE
  main.cpp
  Compiler.cpp)

add_subdirectory(Lex)
add_subdirectory(AST)
add_subdirectory(Symbol)
add_subdirectory(Parse)
add_subdirectory(CodeGen)
