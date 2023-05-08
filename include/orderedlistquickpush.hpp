#ifndef __QUICKPUSHORDEREDLIST_HPP__
#define __QUICKPUSHORDEREDLIST_HPP__

#include "orderedList.hpp"

template <class T>
class QuickPushOrderedList : public OrderedList<T>
{
    public:
        QuickPushOrderedList(std::function<int(const T*, const T*)> cmp_func_);
        ~QuickPushOrderedList();
        virtual T &Pop(); // O(n) time complexity
        virtual void Push(T &data_); // O(1) time complexity
};

////////// Implementations ////////////////////////////////////////////////////


template <class T>
T& QuickPushOrderedList<T>::Pop()
{
    return  OrderedList<T>::Pop();
}

template <class T>
void QuickPushOrderedList<T>::Push(T &data_)
{
    volatile ListNode<T>* new_node = new ListNode<T>(data_);

    std::shared_lock<std::shared_mutex> shared_lock(*this->GetSharedInsertMutex()); 
    std::unique_lock<std::mutex> lock(*this->GetMutex());

    new_node->next = this->Begin();
    this->SetBegin(new_node);
    
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
