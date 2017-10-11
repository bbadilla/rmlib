#ifndef NODE_H
#define NODE_H

#include <cstdlib>

template <class T>
class Node
{
public:
    Node<T>* next;
    T value;

    Node(T _value);
    ~Node();

    Node<T>& operator =(T key);
};

#endif //NODE_H