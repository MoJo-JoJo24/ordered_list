#ifndef __QUICKPOPORDEREDLIST_HPP__
#define __QUICKPOPORDEREDLIST_HPP__

#include "orderedList.hpp"

/*
* This is an implementation of a singly linked list with node-level locks to
* ensure mutual exclusion. After inheriting from the base class, only Pop
* is modified to operate on O(1) complexity by deleting nodes at the beginning 
* of the list.
* The structure looks like this: head_ -> node_0 -> node_1 -> ... -> node_n 
* Note that head_ is a dummy node.
*/

template <class T>
class QuickPopOrderedList : public OrderedList<T>
{
    public:
        QuickPopOrderedList(std::function<int(const T*, const T*)> cmp_func_);
        ~QuickPopOrderedList();
        virtual T *Pop(); // O(1) time complexity
        virtual void Push(T *data_); // O(n) time complexity
};

////////////////////////////////////////////////////////////////////////////////
////////// Implementations ////////////////////////////////////////////////////

template <class T>
T* QuickPopOrderedList<T>::Pop()
{
    sem_wait(this->GetSemaphore());

    ListNode<T> *prev = this->GetBegin();
    std::unique_lock<std::shared_mutex> prev_lock(prev->m_rwmutex);
    ListNode<T> *curr = prev->next;
    std::unique_lock<std::shared_mutex> curr_lock(curr->m_rwmutex);

    T *data = curr->val;
    prev->next = curr->next;  
    curr_lock.unlock();
    delete curr;

    return data;
}

template <class T>
void QuickPopOrderedList<T>::Push(T *data_)
{
    return OrderedList<T>::Push(data_);
}


template <class T>
QuickPopOrderedList<T>::QuickPopOrderedList(std::function<int(const T*, const T*)> cmp_func_)
: OrderedList<T>(cmp_func_)
{}

template <class T>
QuickPopOrderedList<T>::~QuickPopOrderedList()
{}


#endif /* __QUICKPOPORDEREDLIST_HPP__ */
