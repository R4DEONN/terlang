add_library(lexer
    Lexer/Lexer.cpp
    Lexer/Lexer.h
    Lexer/LexerReader.cpp
    Lexer/LexerReader.h
        SyntaxAnalizator/SyntaxAnalizator.h
        SyntaxAnalizator/SyntaxAnalizator.cpp
        Token.h
        TokenType.h
        TokenUtils.h
)
target_include_directories(lexer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})