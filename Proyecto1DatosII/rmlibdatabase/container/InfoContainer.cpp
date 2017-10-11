#include <cstring>
#include "InfoContainer.h"

InfoContainer::InfoContainer()
{
    _key;
    _value;
    _size = 0;
    _reference = 1;
}
InfoContainer::~InfoContainer(){

}


void InfoContainer::operator delete(void * ptr)
{
    free(ptr);
}

bool InfoContainer::operator!=(InfoContainer container)
{
    if ((container.get_key() != _key))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool InfoContainer::operator==(std::string key)
{
    if ((key == _key))
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool InfoContainer::operator!=(std::string key)
{
    if ((key != _key))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool InfoContainer::operator==(InfoContainer container)
{
    if ((container.get_key() == _key))
    {
        return true;
    }
    else
    {
        return false;
    }
}

InfoContainer& InfoContainer::operator=(InfoContainer container)
{
    _key = container.get_key();
    _value = container.get_value();
    _size = container.get_size();
}

std::string InfoContainer::get_key() {
    return _key;
}

void InfoContainer::set_key(std::string key) {
    _key = key;
}

int InfoContainer::get_size() const {
    return _size;
}

void InfoContainer::set_size(int _size) {
    InfoContainer::_size = _size;
}

std::string InfoContainer::get_value() const {
    return _value;
}

void InfoContainer::set_value(std::string value) {
    InfoContainer::_value = value;
}

int InfoContainer::get_reference()
{
    return _reference;
}

void InfoContainer::decrease_reference()
{
    _reference = 0;
}

std::string InfoContainer::to_string()
{
    return _key + "," + std::to_string(_size) + "," + _value + ",";
}