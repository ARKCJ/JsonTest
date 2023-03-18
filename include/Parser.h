#pragma once

#include <string>
#include "Json.h"


namespace test
{
namespace json
{
    class Parser final
    {
    private:
        std::string m_String;
        size_t m_Index;
    public:
        Parser();
        Parser(const std::string& str);
        ~Parser(){}

        void Load(const std::string& str);
        Json Parse();

    private:
        char GetNextToken();
        void PassAnnotation();
        void LocateIndex(size_t index);
        bool IsInBorder(size_t index);

        Json ParseNull();
        Json ParseBool();
        Json ParseNumber();
        Json ParseString();
        Json ParseArray();
        Json ParseObject();
    };
}
}