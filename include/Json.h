#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stddef.h>

namespace test
{
namespace json
{
    class Json final
    {
    public:
        enum class Type
        {
            JSON_NULL = 0,
            JSON_BOOL,
            JSON_INT,
            JSON_DOUBLE,
            JSON_STRING,
            JSON_ARRAY,
            JSON_OBJECT
        };
        typedef std::vector<Json>::iterator iterator;
        typedef std::vector<Json>::const_iterator const_iterator;
        typedef std::vector<Json> ArrayType;
        typedef std::map<std::string, Json> ObjectType;
    private:
        Type m_Type;
        std::shared_ptr<void> m_ObjectPtr;
    public:
        Json();
        Json(Type type);
        Json(bool value);
        Json(int value);
        Json(double value);
        Json(const char* value);
        Json(const std::string& value);
        Json(const Json& other);
        ~Json(){}

        Type GetType() const;

        bool IsNull() const;
        bool IsBool() const;
        bool IsInt() const;
        bool IsDouble() const;
        bool IsString() const;
        bool IsArray() const;
        bool IsObject() const;

        bool AsBool() const;
        int AsInt() const;
        double AsDouble() const;
        std::string AsString() const;

        // return the number of values in array or object
        size_t Size() const;
        // return true if empty array, empty object, or null, otherwise, false.
        bool Empty() const;

        // return true if array and array has the index, otherwise, false.
        bool Has(size_t index);
        // return true if object and the object has key, otherwise, false.
        bool Has(const std::string& key);

        // Remove, if type is array and array has index.
        void Remove(size_t index);
        // Remove, if type is object and object has key.
        void Remove(const std::string& key);

        // append value to array at the end.
        Json& Append(const Json& value);

        Json& operator=(const Json& other);
        bool operator==(const Json& other);
        bool operator!=(const Json& other);

        Json& operator[](size_t index);
        Json& operator[](std::string& key);

        operator bool();
        operator int();
        operator double();
        operator std::string();

        void Parse(const std::string& str);
        std::string Str() const;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
    };
}
}