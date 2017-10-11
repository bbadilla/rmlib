#ifndef RMREF_H_H
#define RMREF_H_H

#include <cstdlib>
#include <iostream>
#include <cstring>

class rmRef_H
{
private:
    std::string _key;
    int _size;
    void* _value;
    std::string _svalue;

public:

    rmRef_H();
    rmRef_H(std::string key, void* value, int size);

    bool operator ==(rmRef_H ref_h);
    bool operator !=(rmRef_H ref_h);
    void operator delete(void*);
    rmRef_H& operator =(rmRef_H);

    bool compare(std::string ptr1, std::string ptr2);
    void* deserialize(std::string encoded);

    // Getters and setters
    std::string get_key();
    void set_key(std::string key);
    int get_size();
    void set_size(int _size);
    void *get_value();
    void set_value(void *value);
    std::string get_svalue();
    void set_svalue(std::string value);
};

#endif //RMREF_H_H
