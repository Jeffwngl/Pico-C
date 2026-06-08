#include "preprocess.h"

// TODO: implement pragma

std::string PreProcessor::processSource(const std::string& src,
                                        const std::filesystem::path& currentDir)
{
    // TODO: get rid of trimmed for easier debuggin
    std::string cleaned = removeComments(src);
    std::string output;

    std::istringstream f(cleaned);
    std::string line;

    while (std::getline(f, line))
    {
        std::string trimmed = trim(line);

        if (!trimmed.empty() && trimmed[0] == '#')
        {
            size_t curr = 1; // skip #

            std::string directive = readWord(trimmed, curr);

            if (directive == "ifdef")
            {
                handleIfdef(trimmed, curr);
                continue;
            }

            if (directive == "ifndef")
            {
                handleIfndef(trimmed, curr);
                continue;
            }

            if (directive == "else")
            {
                handleElse();
                continue;
            }

            if (directive == "endif")
            {
                handleEndif();
                continue;
            }

            // other directives are ignored inside inactive blocks
            if (!isActive())
            {
                continue;
            }

            if (directive == "define")
            {
                handleDefine(trimmed, curr);
                continue;
            }

            if (directive == "include")
            {
                std::string includeName =
                    handleInclude(trimmed, curr, currentDir);

                // replace this with reading/preprocessing the file
                output += includeName;
                output += '\n';

                continue;
            }

            throw std::runtime_error("unknown preprocessor directive: #" +
                                     directive);
        }
        if (isActive())
        {
            output += replaceMacros(trimmed);
            output += '\n';
        }
    }

    if (!conditions.empty())
    {
        throw std::runtime_error("Unterminated #if/#ifdef/#ifndef block.");
    }

    return output;
};

std::string PreProcessor::removeComments(const std::string& src)
{
    std::string output;
    bool inString = false; // for // in strings

    for (std::size_t i = 0; i < src.size(); ++i)
    {
        char c = src[i];

        if (c == '"' && (i == 0 || src[i - 1] != '\\'))
        {
            inString = !inString;
            output += c;
            continue;
        }

        if (!inString && c == '/' && i + 1 < src.size() && src[i + 1] == '/')
        {
            i += 2;
            while (i < src.size() && src[i] != '\n')
            {
                i++;
            }

            if (i < src.size() && src[i] == '\n')
            {
                output += '\n';
            }
            continue;
        }
        else
        {
            output += src[i];
        }
    }

    return output;
}

// TODO: replace later with proper macro replacements
std::string PreProcessor::replaceMacros(const std::string& line)
{
    std::string output;
    bool inString = false;
    bool inChar = false;

    for (std::size_t i = 0; i < line.size();)
    {
        char c = line[i];

        if (c == '"' && !inChar)
        {
            bool escaped = i > 0 && line[i - 1] == '\\';
            if (!escaped)
            {
                inString = !inString;
            }

            output += c;
            i++;
            continue;
        }

        if (c == '\'' && !inString)
        {
            bool escaped = i > 0 && line[i - 1] == '\\';
            if (!escaped)
            {
                inChar = !inChar;
            }

            output += c;
            i++;
            continue;
        }

        if (!inString && !inChar &&
            (std::isalpha(static_cast<unsigned char>(c)) || c == '_'))
        {
            std::size_t start = i;
            i++;

            while (i < line.size() &&
                   (std::isalnum(static_cast<unsigned char>(line[i])) ||
                    line[i] == '_'))
            {
                i++;
            }

            std::string ident = line.substr(start, i - start);

            auto it = macros.find(ident);

            if (it != macros.end())
            {
                output += it->second.replacement;
            }
            else
            {
                output += ident;
            }

            continue;
        }

        output += c;
        i++;
    }

    return output;
}

// e.g. #define PI 3.14 // will not work for 1 + 2
// TODO: make it work for other cases
void PreProcessor::handleDefine(const std::string& line, std::size_t curr)
{
    // while (curr < line.size() &&
    //        std::isspace(static_cast<unsigned char>(line[curr])))
    // {
    //     curr++;
    // }

    std::string name = readWord(line, curr);

    if (name.empty())
        throw std::runtime_error("expected macro name after #define");

    while (curr < line.size() &&
           std::isspace(static_cast<unsigned char>(line[curr])))
    {
        curr++;
    }

    std::string value = line.substr(curr);

    macros[name] = Macro{value};

    // TODO: call the above function replace macros

    return;
}

std::string PreProcessor::handleInclude(
    const std::string& line, std::size_t curr,
    const std::filesystem::path& currentFile)
{
    // manually parsing this is easier
    while (curr < line.size() &&
           std::isspace(static_cast<unsigned char>(line[curr])))
    {
        curr++;
    }

    if (curr >= line.size())
        throw std::runtime_error("expected include path after #include");

    bool quoted = false;
    char endChar = '\0';

    if (line[curr] == '"')
    {
        quoted = true;
        endChar = '"';
        curr++;
    }
    else if (line[curr] == '<')
    {
        quoted = false;
        endChar = '>';
        curr++;
    }
    else
    {
        throw std::runtime_error("expected \"file\" or <file> after #include");
    }

    std::size_t start = curr;

    while (curr < line.size() && line[curr] != endChar)
    {
        curr++;
    }

    if (curr >= line.size())
        throw std::runtime_error("unterminated include path");

    std::string includeName = line.substr(start, curr - start);
    // filepath to quoted or angled file name
    // find real file path and open and preprocess that file then return
    // contents so they can replace the #include line

    return includeName; // placeholder for now
}

bool PreProcessor::isActive() const
{
    return conditions.empty() || conditions.back().currActive;
}

void PreProcessor::handleIfdef(const std::string& line, std::size_t curr)
{
    std::string name = readWord(line, curr);

    if (name.empty())
        throw std::runtime_error("expected macro name after #ifdef");

    bool parentActive = isActive();
    bool condition = macros.find(name) != macros.end();

    conditions.push_back(
        {parentActive, condition, parentActive && condition, false});
}

void PreProcessor::handleIfndef(const std::string& line, std::size_t curr)
{
    std::string name = readWord(line, curr);

    if (name.empty())
        throw std::runtime_error("expected macro name after #ifndef");

    bool parentActive = isActive();
    bool condition = macros.find(name) == macros.end();

    conditions.push_back(
        {parentActive, condition, parentActive && condition, false});
}

void PreProcessor::handleElse()
{
    if (conditions.empty())
        throw std::runtime_error("#else without #if/#ifdef/#ifndef");

    ConditionalFrame& recent = conditions.back();

    if (recent.seenElse)
        throw std::runtime_error("duplicate #else");

    recent.seenElse = true;
    recent.currActive = recent.parentActive && !recent.used;
    recent.used = true;
}

void PreProcessor::handleEndif()
{
    if (conditions.empty())
        throw std::runtime_error("#endif without #if/#ifdef/#ifndef");

    conditions.pop_back();
}

/**
 * Helpers
 */

// trims white space at start and end of strings
std::string PreProcessor::trim(const std::string& str)
{
    std::size_t start = 0;

    while (start < str.size() &&
           std::isspace(static_cast<unsigned char>(str[start])))
    {
        start++;
    }

    std::size_t end = str.size();

    while (end > start &&
           std::isspace(static_cast<unsigned char>(str[end - 1])))
    {
        end--;
    }

    return str.substr(start, end - start);
}

std::string PreProcessor::readWord(const std::string& line, std::size_t& pos)
{
    while (pos < line.size() &&
           std::isspace(static_cast<unsigned char>(line[pos])))
    {
        pos++;
    }

    std::size_t start = pos;

    while (pos < line.size() &&
           !std::isspace(static_cast<unsigned char>(line[pos])))
    {
        pos++;
    }

    return line.substr(start, pos - start);
}