#ifndef LISTHANDLER_H
#define LISTHANDLER_H

#include "List.cpp"

template <class T>
class ListHandler
{
private:
    LinkedList<T> list;
public:
    ListHandler();
    T operator[](int index);

    void add(T element);
    void erase(std::string key);
    T find(std::string key);
    T getPosition(int index);

    bool isInList(std::string key);
    int size();

    int ssize();
    void more_ssize();
    void less_ssize();
};

#endif //LISTHANDLER_H