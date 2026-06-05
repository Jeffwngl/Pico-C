#include "lexer/lexer.h"
#include "parser/parser.h"
#include "utils/print_ast.h"
#include "utils/print_type.h"
#include "utils/utils.h"
#include <iostream>
#include <string>

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

    std::cout << "After Lexer: " << '\n';

    std::cout << '\n';

    for (auto& it : v)
    {
        std::cout << it.val << ' ' << tokenTypeToString(it.type) << '\n';
    }

    std::cout << '\n';

    std::cout << "After Parser: " << '\n';

    Parser parser(v);

    std::unique_ptr<Program> p = parser.parse();

    for (auto& it : p->statements)
    {
        printStmt(it.get());
        std::cout << "\n";
    }

    return 0;
}