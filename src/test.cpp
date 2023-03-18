#include <iostream>
#include <fstream>
#include <sstream>
#include "Json.h"


using namespace test::json;
void test1()
{
    Json v1 = true;
    Json v2 = 123;
    Json v3 = 1.23;
    Json v4 = "hello world";
    std::cout << v1.Str() << std::endl;
    std::cout << v2.Str() << std::endl;
    std::cout << v3.Str() << std::endl;
    std::cout << v4.Str() << std::endl;

    bool b = v1;
    int i = v2;
    double f = v3;
    const std::string& s = v4;
    std::cout << b << std::endl;
    std::cout << i << std::endl;
    std::cout << f << std::endl;
    std::cout << s << std::endl;

    Json null;
    std::cout << null.Str() << std::endl;
}

void test2()
{
    Json arr;
    arr[0] = 123;
    arr[1] = 1.23;
    arr[2] = "hello world";
    arr[5] = true;
    arr.Append(123);
    arr.Append(1.23);
    arr.Append("hello world");
    std::cout << arr.Str() << std::endl;

    arr.Remove(1);
    std::cout << arr.Str() << std::endl;
}

void test3()
{
    const char* fileName = "./test.json";
    std::ifstream fin(fileName);
    std::stringstream ss;
    ss << fin.rdbuf();
    ss.flush();
    Json json;
    const std::string& data = ss.str();
    json.Parse(data);
    std::cout << json.Str() << std::endl;
}

int main()
{
    try{
        // test1();
        // test2();
        test3();
    }catch(std::exception& e){
        std::cout << "catch exception: " << e.what() << std::endl;
    }
}