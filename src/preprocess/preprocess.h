// handles preprocesser parse, e.g. #inclues, IFDEF ...

#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>

struct Macro
{
    std::string replacement;
};

struct ConditionalFrame
{
    bool parentActive;
    bool used;
    bool currActive;
    bool seenElse;
};

class PreProcessor
{
public:
    std::string processSource(const std::string& src,
                              const std::filesystem::path& currentDir);

private:
    std::unordered_set<std::string> includes;
    std::unordered_map<std::string, Macro> macros;
    std::vector<ConditionalFrame> conditions;

    std::string removeComments(const std::string& src);

    bool isActive() const;

    std::string handleInclude(const std::string& line, std::size_t curr,
                              const std::filesystem::path& currentFile);
    std::string replaceMacros(const std::string& line);

    void handleDefine(const std::string& line, std::size_t curr);
    void handleIfdef(const std::string& line, std::size_t curr);
    void handleIfndef(const std::string& line, std::size_t curr);
    void handleElse();
    void handleEndif();

    // helpers
    // std::string trim(const std::string& str);
    std::string readWord(const std::string& line, std::size_t& pos);
};

#endif