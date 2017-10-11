#ifndef INFOCONTAINER_H
#define INFOCONTAINER_H

#include <cstdlib>
#include <iostream>

class InfoContainer
{
private:
    std::string _key;
    int _size;
    std::string _value;
    int _reference;
public:

    InfoContainer();
    ~InfoContainer();

    bool operator ==(InfoContainer container);
    bool operator==(std::string key);
    bool operator !=(InfoContainer container);
    bool operator !=(std::string key);
    void operator delete(void*);
    InfoContainer& operator =(InfoContainer container);

    std::string to_string();

    // Getters and setters
    std::string get_key();
    void set_key(std::string key);
    int get_size() const;
    void set_size(int _size);
    std::string get_value() const;
    void set_value(std::string _value);
    int get_reference();
    void decrease_reference();
};



#endif //INFOCONTAINER_H