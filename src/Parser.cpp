#include "Parser.h"
#include <iostream>

namespace test
{
namespace json
{
    Parser::Parser():m_Index(0){}

    Parser::Parser(const std::string& str):m_Index(0), m_String(str){}

    void Parser::Load(const std::string& str)
    {
        m_Index = 0;
        m_String = str;
    }

    void Parser::PassAnnotation()
    {
        if(IsInBorder(m_Index) && IsInBorder(m_Index + 1) 
            && 
        m_String[m_Index] == '/' && m_String[m_Index + 1] == '/'){
            while(IsInBorder(m_Index) && m_String[m_Index] != '\n'){
                ++m_Index;
            }
        }
    }

    char Parser::GetNextToken()
    {
        while(IsInBorder(m_Index) && (
            m_String[m_Index] == ' ' 
            || m_String[m_Index] == '\r' 
            || m_String[m_Index] == '\n' 
            || m_String[m_Index] == '\n' 
            || m_String[m_Index] == '\t'
            || m_String[m_Index] == '/'
        )){
            PassAnnotation();
            if(m_String[m_Index] != '/'){
                ++m_Index;
            }
        }
        if(m_Index == m_String.size()){
            throw std::logic_error("unexpected end of input.");
        }
        return m_String[m_Index++];
    }

    void Parser::LocateIndex(size_t index)
    {
        m_Index = index;
    }

    Json Parser::Parse()
    {
        char ch = GetNextToken();
        switch (ch)
        {
        case 'n':
            --m_Index;
            return ParseNull();
        case 't':
        case 'f':
            --m_Index;
            return ParseBool();
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            --m_Index;
            return ParseNumber();
        case '"':
            --m_Index;
            return ParseString();
        case '[':
            --m_Index;
            return ParseArray();
        case '{':
            --m_Index;
            return ParseObject();
        default:
            break;
        }
        throw std::logic_error("Parser::Parse(): unexpected character in parse json");
    }

    bool Parser::IsInBorder(size_t index)
    {
        if(index < 0 || index >= m_String.size()){
            return false;
        }
        return true;
    }

    Json Parser::ParseNull()
    {
        if(IsInBorder(m_Index + 3) && m_String.compare(m_Index, 4, "null") == 0)
        {
            LocateIndex(m_Index + 4);
            return Json();
        }
        throw std::logic_error("Parser::ParseNull(): parse null error.");
    }

    Json Parser::ParseBool()
    {
        if(IsInBorder(m_Index + 3) && m_String.compare(m_Index, 4, "true") == 0)
        {
            LocateIndex(m_Index + 4);
            return Json(true);
        }else if(IsInBorder(m_Index + 4) && m_String.compare(m_Index, 5, "false") == 0){
            LocateIndex(m_Index + 5);
            return Json(false);
        }
        throw std::logic_error("Parser::ParseBool(): parse bool error.");
    }

    Json Parser::ParseNumber()
    {
        size_t numLen = 0;
        if(IsInBorder(m_Index + numLen) && m_String[m_Index + numLen] == '-'){
            ++numLen;
        }
        if(IsInBorder(m_Index + numLen) == false || !(m_String[m_Index + numLen] >= '0' && m_String[m_Index + numLen] <= '9')){
            throw std::logic_error("Parser::ParseNumber(): invalid character in number.");
        }
        while(IsInBorder(m_Index + numLen) && (m_String[m_Index + numLen] >= '0' && m_String[m_Index + numLen] <= '9')){
            ++numLen;
        }
        if(!(IsInBorder(m_Index + numLen) && m_String[m_Index + numLen] == '.')){
            // int
            Json obj(std::stoi(std::string(m_String, m_Index, numLen))); 
            LocateIndex(m_Index + numLen);
            // std::cout << obj.Str() << std::endl;
            return obj;
        }
        // double
        ++numLen;
        if(!IsInBorder(m_Index + numLen) || !(m_String[m_Index + numLen] >= '0' && m_String[m_Index + numLen] <= '9')){
            throw std::logic_error("Parser::ParseNumber(): invalid character in number.");
        }
        while(IsInBorder(m_Index + numLen) && (m_String[m_Index + numLen] >= '0' && m_String[m_Index + numLen] <= '9')){
            ++numLen;
        }
        Json obj(std::stod(std::string(m_String, m_Index, numLen)));
        LocateIndex(m_Index + numLen);
        // std::cout << obj.Str() << std::endl;
        return obj;
    }

    Json Parser::ParseString()
    {
        size_t strLen = 0;
        if(!(IsInBorder(m_Index + strLen) && m_String[m_Index + strLen] ==  '\"')){
            throw std::logic_error("Parser::ParseString(): unexpected input in string.");
        }
        ++strLen;
        while(IsInBorder(m_Index + strLen)){
            char ch = m_String[m_Index + strLen];
            if(ch == '\"'){
                break;
            }
            if (ch == '\\')
            {
                if(!IsInBorder(m_Index + strLen + 1)){
                    throw std::logic_error("Parser::ParseString(): unexpected end of input in string.");
                }
                ++strLen;
                ch = m_String[m_Index + strLen];
                switch (ch)
                {
                    case 'u':
                        if(!IsInBorder(m_Index + strLen + 4)){
                            throw std::logic_error("Parser::ParseString(): unexpected input in string.");
                        }
                        strLen += 4;
                        break;
                    default:
                        break;
                }
            }
            ++strLen;
        }
        if(IsInBorder(m_Index + strLen) == false){
            throw std::logic_error("Parser::ParseString(): unexpected end of input in string.");
        }
        ++strLen;
        LocateIndex(m_Index + strLen);
        // std::cout << m_String.substr(m_Index - strLen + 1, strLen - 2) << std::endl;
        return Json(m_String.substr(m_Index - strLen + 1, strLen - 2));
    } 

    Json Parser::ParseArray()
    {
        Json arr(Json::Type::JSON_ARRAY);
        if(!(IsInBorder(m_Index) && m_String[m_Index] == '[')){
            throw std::logic_error("Parser::ParseArray(): unexpected end of input in array.");
        }
        ++m_Index;
        char ch = GetNextToken();
        while(ch != ']'){
            --m_Index;
            arr.Append(Parse());
            ch = GetNextToken();
            if(ch == ']'){
                break;
            }
            if(ch != ','){
                throw std::logic_error("Parser::ParseArray(): expected ',' in array");
            }
            ch = GetNextToken();
        }
        // std::cout << arr.Str() << std::endl;
        return arr;
    }

    Json Parser::ParseObject()
    {
        Json obj(Json::Type::JSON_OBJECT);
        if(!(IsInBorder(m_Index) && m_String[m_Index] == '{')){
            throw std::logic_error("Parser::ParseObject(): unexpected end of input in object.");
        }
        ++m_Index;
        char ch = GetNextToken();
        --m_Index;
        while(ch != '}'){
            if (ch != '"'){
                throw std::logic_error("Parser::ParseObject(): expected '\"' in object");
            }
            std::string key = ParseString();
            ch = GetNextToken();
            if (ch != ':'){
                throw std::logic_error("Parser::ParseObject(): expected ':' in object");
            }
            // std::cout << "key: " << key << std::endl;
            obj[key] = Parse();
            // std::cout << "val: " <<obj[key].Str() << std::endl;
            ch = GetNextToken();
            if(ch == '}'){
                break;
            }
            if(ch != ','){
                throw std::logic_error("Parser::ParseObject(): expected ',' in object");
            }
            ch = GetNextToken();
            --m_Index;
        }
        return obj;
    }
}
} // namespace test
