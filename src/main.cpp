#include "lexer/lexer.h"
#include "utils/utils.h"
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: picoc <file-name.c>\n";
        return 1;
    }

    std::string src = readFile(argv[1]);

    Lexer lexer(src, "examples/return_2.c");

    std::vector<Token> v = lexer.tokenize();

    for (auto& it : v)
    {
        std::cout << it.val << std::endl;
    }

    return 0;
}