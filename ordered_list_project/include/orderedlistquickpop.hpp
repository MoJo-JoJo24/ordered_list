#ifndef __QUICKPOPORDEREDLIST_HPP__
#define __QUICKPOPORDEREDLIST_HPP__

#include "orderedList.hpp"

template <class T>
class QuickPopOrderedList : public OrderedList<T>
{
    public:
        QuickPopOrderedList(std::function<int(const T*, const T*)> cmp_func_);
        ~QuickPopOrderedList();
        virtual T &Pop(); // O(1) time complexity
        virtual void Push(T &data_); // O(n) time complexity
};

////////// Implementations ////////////////////////////////////////////////////


template <class T>
T& QuickPopOrderedList<T>::Pop()
{
    sem_wait(this->GetSemaphore());

    std::unique_lock<std::shared_mutex> shared_lock(*this->GetSharedSearchMutex());
    std::unique_lock<std::shared_mutex> shared_lock2(*this->GetSharedInsertMutex());

    volatile ListNode<T>* holding_ptr = this->Begin();
    
    T &data = holding_ptr->val;
    this->SetBegin(holding_ptr->next);
    
    delete holding_ptr;

    return data;
}

template <class T>
void QuickPopOrderedList<T>::Push(T &data_)
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
