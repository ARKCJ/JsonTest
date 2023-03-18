#include <stdexcept>
#include <sstream>
#include <iostream>
#include "Json.h"
#include "Parser.h"

namespace test
{
namespace json
{
    Json::Json(): m_Type(Json::Type::JSON_NULL){}

    Json::Json(Type type): m_Type(type)
    {
        switch (m_Type)
        {
            case Type::JSON_NULL:
                break;
            case Type::JSON_BOOL:
                m_ObjectPtr = std::make_shared<bool>(false);
                break;
            case Type::JSON_INT:
                m_ObjectPtr = std::make_shared<int>(0);
                break;
            case Type::JSON_DOUBLE:
                m_ObjectPtr = std::make_shared<double>(0.0);
                break;
            case Type::JSON_STRING:
                m_ObjectPtr = std::make_shared<std::string>("");
                break;
            case Type::JSON_ARRAY:
                m_ObjectPtr = std::make_shared<std::vector<Json>>();
                break;
            case Type::JSON_OBJECT:
                m_ObjectPtr = std::make_shared<std::map<std::string, Json>>();
                break;
            default:
                break;
        }
    }

    Json::Json(bool value)
    :m_Type(Type::JSON_BOOL), m_ObjectPtr(new bool{value}){}

    Json::Json(int value)
    :m_Type(Type::JSON_INT), m_ObjectPtr(new int{value}){}

    Json::Json(double value)
    :m_Type(Type::JSON_DOUBLE), m_ObjectPtr(new double{value}){}

    Json::Json(const char* value)
    :m_Type(Type::JSON_STRING), m_ObjectPtr(new std::string(value)){}

    Json::Json(const std::string& value)
    :m_Type(Type::JSON_STRING), m_ObjectPtr(new std::string(value)){}

    Json::Json(const Json& other)
    :m_Type(other.m_Type), m_ObjectPtr(other.m_ObjectPtr){}

    Json::iterator Json::begin()
    {
        if(m_Type == Type::JSON_ARRAY){
            return (*reinterpret_cast<std::vector<Json>*>(m_ObjectPtr.get())).begin();
        }
        throw std::logic_error("function Json::begin get iterator error.");
    }

    Json::iterator Json::end()
    {
        if(m_Type == Type::JSON_ARRAY){
            return (*reinterpret_cast<std::vector<Json>*>(m_ObjectPtr.get())).end();
        }
        throw std::logic_error("function Json::end get iterator error.");
    }

    Json::const_iterator Json::begin() const
    {
        if(m_Type == Type::JSON_ARRAY){
            return (*reinterpret_cast<std::vector<Json>*>(m_ObjectPtr.get())).cbegin();
        }
        throw std::logic_error("function Json::begin get iterator error.");
    }

    Json::const_iterator Json::end() const
    {
        if(m_Type == Type::JSON_ARRAY){
            return (*reinterpret_cast<std::vector<Json>*>(m_ObjectPtr.get())).cend();
        }
        throw std::logic_error("function Json::end get iterator error.");
    }

    Json::Type Json::GetType() const { return m_Type; }

    bool Json::IsNull() const { return m_Type == Type::JSON_NULL; }

    bool Json::IsBool() const { return m_Type == Type::JSON_BOOL; }

    bool Json::IsInt() const { return m_Type == Type::JSON_INT; }

    bool Json::IsDouble() const { return m_Type == Type::JSON_DOUBLE; }

    bool Json::IsString() const { return m_Type == Type::JSON_STRING; }

    bool Json::IsArray() const { return m_Type == Type::JSON_ARRAY; }

    bool Json::IsObject() const { return m_Type == Type::JSON_OBJECT; }

    bool Json::AsBool() const
    {
        if(m_Type == Type::JSON_BOOL){
            return *reinterpret_cast<bool*>(m_ObjectPtr.get());
        }
        throw std::logic_error("function Json::AsBool value type error.");
    }
    
    int Json::AsInt() const
    {
        if(m_Type == Type::JSON_INT){
            return *reinterpret_cast<int*>(m_ObjectPtr.get());
        }
        throw std::logic_error("function Json::AsInt value type error.");
    }
    
    double Json::AsDouble() const
    {
        if(m_Type == Type::JSON_DOUBLE){
            return *reinterpret_cast<double*>(m_ObjectPtr.get());
        }
        throw std::logic_error("function Json::AsDouble value type error.");
    }
    
    std::string Json::AsString() const
    {
        if(m_Type == Type::JSON_STRING){
            return *reinterpret_cast<std::string*>(m_ObjectPtr.get());
        }
        throw std::logic_error("function Json::AsString value type error.");
    }

    size_t Json::Size() const
    {
        switch (m_Type)
        {
        case Type::JSON_ARRAY:
            return (*reinterpret_cast<std::vector<Json>*>(m_ObjectPtr.get())).size();
        case Type::JSON_OBJECT:
            return (*reinterpret_cast<std::map<std::string, Json>*>(m_ObjectPtr.get())).size();
        default:
            break;
        }
        throw std::logic_error("function Json::Size value type error");
    }

    bool Json::Empty() const
    {
        switch (m_Type)
        {
        case Type::JSON_NULL:
            return true;
        case Type::JSON_ARRAY:
            return (*reinterpret_cast<std::vector<Json>*>(m_ObjectPtr.get())).empty();
        case Type::JSON_OBJECT:
            return (*reinterpret_cast<std::map<std::string, Json>*>(m_ObjectPtr.get())).empty();
        default:
            break;
        }
        return false;
    }

    bool Json::Has(size_t index)
    {
        if(m_Type == Type::JSON_ARRAY){
            int t_size = Size();
            if(index >= 0 && index < t_size){
                return true;
            }
        }
        return false;
    }

    bool Json::Has(const std::string& key)
    {
        if(m_Type == Type::JSON_OBJECT){
            auto& t = *reinterpret_cast<std::map<std::string, Json>*>(m_ObjectPtr.get());
            if(t.find(key) != t.end()){
                return true;
            }
        }
        return false;
    }

    void Json::Remove(size_t index)
    {
        if(m_Type != Type::JSON_ARRAY){
            return;
        }
        if(Has(index)){
            (*reinterpret_cast<ArrayType*>(m_ObjectPtr.get())).erase(begin() + index);
        }
    }

    void Json::Remove(const std::string& key)
    {
        if(m_Type != Type::JSON_OBJECT){
            return;
        }
        if(Has(key)){
            (*reinterpret_cast<ObjectType*>(m_ObjectPtr.get())).erase(key);
        }
    }

    Json& Json::Append(const Json& value)
    {
        if(m_Type != Type::JSON_ARRAY){
            m_Type = Type::JSON_ARRAY;
            m_ObjectPtr = std::make_shared<ArrayType>();
        }
        (*reinterpret_cast<ArrayType*>(m_ObjectPtr.get())).push_back(value);
        return *this;
    }

    Json& Json::operator=(const Json& other)
    {
        if(this != &other){
            m_Type = other.m_Type;
            m_ObjectPtr = other.m_ObjectPtr;
        }
        return *this;
    }

    bool Json::operator==(const Json& other)
    {
        if(this == &other){
            return true;
        }
        if(m_Type != other.GetType()){
            return false;
        }
        switch (m_Type)
        {
        case Type::JSON_NULL:
            return true;
        case Type::JSON_BOOL:
            {return (*reinterpret_cast<bool*>(m_ObjectPtr.get())) == (*reinterpret_cast<bool*>(other.m_ObjectPtr.get()));}
        case Type::JSON_INT:
            {return (*reinterpret_cast<int*>(m_ObjectPtr.get())) == (*reinterpret_cast<int*>(other.m_ObjectPtr.get()));}
        case Type::JSON_DOUBLE:
            {return (*reinterpret_cast<double*>(m_ObjectPtr.get())) == (*reinterpret_cast<double*>(other.m_ObjectPtr.get()));}
        case Type::JSON_STRING:
            {return m_ObjectPtr == other.m_ObjectPtr;}
        case Type::JSON_ARRAY:
            {return m_ObjectPtr == other.m_ObjectPtr;}
        case Type::JSON_OBJECT:
            // {return (*reinterpret_cast<ObjectType*>(m_ObjectPtr.get())) == (*reinterpret_cast<ObjectType*>(other.m_ObjectPtr.get()));}
        default:
            break;
        }
        return false;
    }

    bool Json::operator!=(const Json& other)
    {
        return !(*this == other);
    }

    Json& Json::operator[](size_t index)
    {
        if (index < 0){
            throw std::logic_error("function Json::operator[int] index less than 0.");
        }
        if(m_Type != Type::JSON_ARRAY){
            m_Type = Type::JSON_ARRAY;
            m_ObjectPtr = std::make_shared<ArrayType>(index + 1);
        }
        if(index >= (*reinterpret_cast<ArrayType*>(m_ObjectPtr.get())).size()){
            (*reinterpret_cast<ArrayType*>(m_ObjectPtr.get())).resize(index + 1);
        }
        return (*reinterpret_cast<ArrayType*>(m_ObjectPtr.get()))[index];
    }

    Json& Json::operator[](std::string& key)
    {
        if(m_Type != Type::JSON_OBJECT){
            m_Type = Type::JSON_OBJECT;
            m_ObjectPtr = std::make_shared<ObjectType>();
        }
        return (*reinterpret_cast<ObjectType*>(m_ObjectPtr.get()))[key];
    }

    Json::operator bool()
    {
        if(m_Type == Type::JSON_BOOL){
            return *reinterpret_cast<bool*>(m_ObjectPtr.get());
        }
        throw std::logic_error("function Json::bool() requires bool value.");
    }

    Json::operator int()
    {
        if(m_Type == Type::JSON_INT){
            return *reinterpret_cast<int*>(m_ObjectPtr.get());
        }
        throw std::logic_error("function Json::int() requires int value.");
    }

    Json::operator double()
    {
        if(m_Type == Type::JSON_DOUBLE){
            return *reinterpret_cast<double*>(m_ObjectPtr.get());
        }
        throw std::logic_error("function Json::double() requires double value.");
    }

    Json::operator std::string()
    {
        if(m_Type == Type::JSON_STRING){
            return *reinterpret_cast<std::string*>(m_ObjectPtr.get());
        }
        throw std::logic_error("function Json::std::string() requires std::string value.");
    }

    std::string Json::Str() const
    {
        std::stringstream ss;
        switch (m_Type)
        {
            case Type::JSON_NULL:
                {
                    ss << "null";
                    break;
                }
            case Type::JSON_BOOL:
                {
                    if(*reinterpret_cast<bool*>(m_ObjectPtr.get()) == true){
                        ss << "true";
                    }else{
                        ss << "false";
                    }
                break;
                }
            case Type::JSON_INT:
                {
                    ss << *reinterpret_cast<int*>(m_ObjectPtr.get());
                    break;
                }
            case Type::JSON_DOUBLE:
                {
                    ss << *reinterpret_cast<double*>(m_ObjectPtr.get());
                    break;
                }
            case Type::JSON_STRING:
                {
                    ss << "\"" + *reinterpret_cast<std::string*>(m_ObjectPtr.get()) + "\"";
                    break;
                }
            case Type::JSON_ARRAY:
                {
                    ss << "[";
                    for(auto it = begin(); it != end(); ++it){
                        if (it != begin())
                        {
                            ss << ",";
                        }
                        ss << (*it).Str();
                    }
                    ss << "]";
                    break;
                }
            case Type::JSON_OBJECT:
                {
                    ss << "{";
                    auto& tmp = *reinterpret_cast<ObjectType*>(m_ObjectPtr.get());
                    for(auto it = tmp.begin(); it != tmp.end(); ++it){
                        if (it != tmp.begin())
                        {
                            ss << ",";
                        }
                        ss << "\"" << it -> first << "\":" << (it -> second).Str();
                        // std::cout << it -> first << std::endl;              
                    }
                    ss << "}";
                    break;
                }
            default:
                break;
        }
        return ss.str();
    }

    void Json::Parse(const std::string& str)
    {
        Parser parser(str);
        *this = parser.Parse();
    }
}
}