#ifndef __QUICKPUSHORDEREDLIST_HPP__
#define __QUICKPUSHORDEREDLIST_HPP__

#include "orderedList.hpp"

/*
* This is an implementation of a singly linked list with node-level locks to
* ensure mutual exclusion. After inheriting from the base class, only Push
* is modified to operate on O(1) complexity by inserting new nodes at the
* beginning of the list.
* The structure looks like this: head_ -> node_0 -> node_1 -> ... -> node_n 
* Note that head_ is a dummy node.
*/

template <class T>
class QuickPushOrderedList : public OrderedList<T>
{
    public:
        QuickPushOrderedList(std::function<int(const T*, const T*)> cmp_func_);
        ~QuickPushOrderedList();
        virtual T *Pop(); // O(n) time complexity
        virtual void Push(T *data_); // O(1) time complexity
};

////////////////////////////////////////////////////////////////////////////////
////////// Implementations ////////////////////////////////////////////////////

template <class T>
T* QuickPushOrderedList<T>::Pop()
{
    return  OrderedList<T>::Pop();
}

template <class T>
void QuickPushOrderedList<T>::Push(T *data_)
{
    ListNode<T>* new_node = new ListNode<T>(data_);

    ListNode<T> *prev = this->GetBegin();
    std::unique_lock<std::shared_mutex> prev_lock(prev->m_rwmutex);
    ListNode<T> *curr = prev->next;
    std::unique_lock<std::shared_mutex> curr_lock;
    if (curr)
    {
        curr_lock = std::unique_lock<std::shared_mutex>(curr->m_rwmutex);
    }

    new_node->next = curr;
    prev->next = new_node;    
       
    sem_post(this->GetSemaphore());          
}

template <class T>
QuickPushOrderedList<T>::QuickPushOrderedList(std::function<int(const T*, const T*)> cmp_func_)
: OrderedList<T>(cmp_func_)
{}

template <class T>
QuickPushOrderedList<T>::~QuickPushOrderedList()
{}


#endif /* __QUICKPUSHORDEREDLIST_HPP__ */
