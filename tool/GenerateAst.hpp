#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using Str = std::string;
using PrintWriter = std::ofstream;

static auto 
defineVisitor(PrintWriter& writer, Str& baseName, std::vector<Str>& types, Str __type__name)
    -> void;

static auto  
defineType(std::ofstream& writer, Str& baseName, Str& visitorName ,Str& className, Str& fieldList)
     -> void;

static auto 
defineAst(Str& outputDir, Str& baseName, std::vector<Str>& types, Str __type__name)
    -> void;


auto trim(const Str& s) -> Str
{
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    return (start == Str::npos) ? "" : s.substr(start, end - start + 1);
}

auto trim(const Str& s, int index) -> Str
{
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    return (start == Str::npos) ? "" : s.substr(start, end - start + index);
}


auto split__(Str& str, Str& delimiter) -> std::vector<Str>
{
    std::vector<Str> tokens;
    size_t pos { 0 };
    size_t prev { 0 };

    while ((pos = str.find(delimiter, prev)) != Str::npos)
    {
        tokens.push_back(str.substr(prev, pos - prev));

        prev = pos + delimiter.length();
    }

    tokens.push_back(str.substr(prev));

    return tokens;
}


auto splitIndexOne(Str s) -> Str
{
    size_t pos = s.find(" ");
    Str second = s.substr(pos + 1);

    return second;
}

auto to_lower(Str& s) -> Str
{
    Str result = s;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) 
    {
        return std::tolower(c);
    });

    return result;
}