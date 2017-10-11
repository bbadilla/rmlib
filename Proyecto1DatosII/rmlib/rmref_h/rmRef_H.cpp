#include "rmRef_H.h"

rmRef_H::rmRef_H()
{
    _key;
    _value = nullptr;
    _size = 0;
}

rmRef_H::rmRef_H(std::string key, void *value, int size)
{
    _key = key;
    _value = value;
    _size = size;
}

bool rmRef_H::compare(std::string ptr1, std::string ptr2)
{
    for (int i = 0; i < ptr1.size(); ++i)
    {
        if (ptr1[i] != ptr2[i])
        {
            return false;
        }

        //std::cout << ptr1[i] << " - " << ptr2[i] << std::endl;
    }

    return true;
}

void* rmRef_H::deserialize(std::string encoded)
{
    char* data = (char*) encoded.c_str();

    return (void*) data;
}

void rmRef_H::operator delete(void * ptr)
{
    free(ptr);
}

bool rmRef_H::operator!=(rmRef_H ref_h)
{
    if (!compare(_svalue, ref_h.get_svalue()) && _size != ref_h.get_size())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool rmRef_H::operator==(rmRef_H ref_h)
{
    if (compare(_svalue, ref_h.get_svalue()) && _size == ref_h.get_size())
    {
        return true;
    }
    else
    {
        return false;
    }
}

rmRef_H& rmRef_H::operator=(rmRef_H ref_h)
{
    _key = ref_h.get_key();
    _value = ref_h.get_value();
    _svalue = ref_h.get_svalue();
    _size = ref_h.get_size();
}

std::string rmRef_H::get_key()
{
    return _key;
}

void rmRef_H::set_key(std::string key)
{
    _key = key;
}

int rmRef_H::get_size()
{
    return _size;
}

void rmRef_H::set_size(int _size)
{
    rmRef_H::_size = _size;
}

void *rmRef_H::get_value()
{
    _value =  deserialize(_svalue);

    return _value;
}

void rmRef_H::set_value(void* value)
{
    rmRef_H::_value = value;
}

std::string rmRef_H::get_svalue()
{
    return _svalue;
}

void rmRef_H::set_svalue(std::string value)
{
    _svalue = value;
}


