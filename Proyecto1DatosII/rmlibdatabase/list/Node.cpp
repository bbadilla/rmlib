#include "Node.h"

template <class T>
Node<T>::Node(T _value)
{
    value = _value;
}

template <class T>
Node<T>::~Node()
{
    delete &value;
}

template <class T>
Node<T>& Node<T>::operator =(T key)
{
    value = key;
}