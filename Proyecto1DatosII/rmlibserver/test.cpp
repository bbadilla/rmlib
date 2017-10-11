#include <iostream>
#include <sstream>

using namespace std;

string void2string(void* ptr, int size);
void* string2void(string value, int size);

int main()
{
    char num = 'q';
    void* ptr = &num;

    cout << void2string(ptr, sizeof(char)) << endl;

    void* prueba = string2void("q", sizeof(char));
    cout << *((char*) prueba) << endl;


    return 0;
}

string void2string(void* ptr, int size)
{
    string result;

    switch (size)
    {
        case sizeof(int):
            result = to_string(*((int*)ptr));
            break;

        case sizeof(double):
            result = to_string(*((double*)ptr));
            break;

        case sizeof(char):
            result = to_string(*((char*)ptr));
            break;

        case sizeof(string):
            result =  *((string*)ptr);
            break;

        default:
            std::cout << "No type" << endl;
    }

    return result;
}

void* string2void(string value, int size)
{
    void* result = nullptr;

    int tempInt = 0;
    double tempDoub;
    char tempChr;
    string tempStr;

    switch (size)
    {
        case sizeof(int):
            tempInt = atoi(value.c_str());
            result = &tempInt;
            break;

        case sizeof(double):
            tempDoub = atof(value.c_str());
            result = &tempDoub;
            break;

        case sizeof(char):
            tempChr = value[0];
            result = &tempChr;
            break;

        case sizeof(string):
            tempStr = value;
            result = &tempStr;
            break;

        default:
            std::cout << "No type" << endl;
    }

    return result;
}