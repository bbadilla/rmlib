#ifndef LISTNODE
#define LISTNODE

#include <mutex>
#include <iostream>
#include "Node.cpp"

template <class T>
class LinkedList
{
private:
    Node<T>* head;
    Node<T>* last;

    int _size = 0;
    int _ssize = 0;

    std::mutex guard;
public:
    LinkedList();
    Node<T>* getHead();
    T operator[](int index);

    void addNode(T value);
    void deleteNode(std::string key);
    T findNode(std::string key);
    T*findPointerNode(std::string key);
    T getPosition(int index);
    bool isInList(std::string key);

    int size();
    int ssize();
    void more_ssize();
    void less_ssize();
};
#endif