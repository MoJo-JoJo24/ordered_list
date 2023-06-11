#ifndef __ORDEREDLIST_HPP__
#define __ORDEREDLIST_HPP__
#include <shared_mutex>        // std::mutex, std::lock_guard
#include <functional> // std::funciton<()>
#include <semaphore.h>


template <class T>
struct ListNode {
    T *val;
    ListNode *next;
    ListNode() : val(), next(nullptr) {}
    ListNode(T *x) : val(x), next(nullptr) {}
    ListNode(T *x, ListNode *next) : val(x), next(next) {}
    mutable std::shared_mutex m_rwmutex;
 };


template <class T>
class QuickPushOrderedList; 

template <class T>
class QuickPopOrderedList; 

/*
* This is an implementation of a singly linked list with node-level locks to
* ensure mutual exclusion. 
* The structure looks like this: head_ -> node_0 -> node_1 -> ... -> node_n 
* Note that head_ is a dummy node.
*/


template <class T>
class OrderedList
{
    public: 
        OrderedList() = delete;
        OrderedList(std::function<int(const T*, const T*)> cmp_func_);
        virtual ~OrderedList();
        virtual T *Pop();
        virtual void Push(T *data_);
      
        sem_t *GetSemaphore();
        ListNode<T> *GetBegin();
    private:
        int SearchNodePosition(T *data_);
        std::function<int(const T*, const T*)> m_cmp_func_;
        ListNode<T> *m_head;
        mutable sem_t m_sem;

        


};

////////////////////////////////////////////////////////////////////////////////
////////// Implementations ////////////////////////////////////////////////////


template <class T>
OrderedList<T>::OrderedList(std::function<int(const T*, const T*)> cmp_func_) 
: m_cmp_func_(cmp_func_), m_head(new ListNode<T>())
{
    sem_init(&m_sem, 0, 0);
}

template <class T>
OrderedList<T>::~OrderedList()
{
    ListNode<T> *holding_ptr = m_head;
    while (nullptr != holding_ptr) //iterate and delete until arriving at the tail
    {
        ListNode<T> *tmp = holding_ptr;
        holding_ptr = holding_ptr->next;
        delete tmp;
    }
    sem_destroy(&m_sem);
}

/*
* Insert a node with value data_.
*
* To ensure mutual exclusion, the locks of the current node and the
* previous nodes must be acquired for insertion to work. As soon as the
* locks are acquired the code does the following:
*
*      | prev -> node | prev -> node | prev    node |
*      |              |          ^   |   v      ^   |
*      |   new_node   |   new_node   |   new_node   |
*/

template <class T>
void OrderedList<T>::Push(T *data_)
{
    int position = SearchNodePosition(data_);
    ListNode<T>* new_node = new ListNode<T>(data_);

    ListNode<T> *prev = m_head;
    std::unique_lock<std::shared_mutex> prev_lock(prev->m_rwmutex);
    ListNode<T> *curr = prev->next;
    std::unique_lock<std::shared_mutex> curr_lock;
    if (curr)
    {
        curr_lock = std::unique_lock<std::shared_mutex>(curr->m_rwmutex);
    }

    if (0 != position)
    {
        for (int i = 0; i < position - 1 && curr != nullptr; ++i) 
        {
            prev = curr;
            curr = curr->next;
            prev_lock.swap(curr_lock);
            if (curr)
            {
                curr_lock = std::unique_lock<std::shared_mutex>(curr->m_rwmutex);
            }
        }
    }
    new_node->next = curr;
    prev->next = new_node;    
    
    sem_post(&m_sem); 
}

/*
* Pop the node at position.
*
* To ensure mutual exclusion, follow the steps from insert(). As soon as
* the locks are acquired the code does roughly the following:
*
*    (*)     (*)            (*)     (*)            (*)
*  | prev -> node -> next | prev    node -> next | prev ---------> next |
*  |                      |   v--------------^   |                      |
*
* (*) highlights the nodes whose locks are acquired.
*/

template <class T>
T *OrderedList<T>::Pop()
{
    sem_wait(&m_sem);

    int position = SearchNodePosition(nullptr);

    ListNode<T> *prev = m_head;
    std::unique_lock<std::shared_mutex> prev_lock(prev->m_rwmutex);
    ListNode<T> *curr = prev->next;
    std::unique_lock<std::shared_mutex> curr_lock;
    if (curr)
    {
        curr_lock = std::unique_lock<std::shared_mutex>(curr->m_rwmutex);
    }
    if (0 != position)
    {
        for (int i = 0; i < position - 1 && curr != nullptr; ++i) 
        {
            prev = curr;
            curr = curr->next;
            prev_lock.swap(curr_lock);
            if (curr)
            {
                curr_lock = std::unique_lock<std::shared_mutex>(curr->m_rwmutex);
            }
        }
    }
    if (nullptr == curr)
    {
        return nullptr;
    }

    T *data = curr->val;
    prev->next = curr->next;  
    curr_lock.unlock();
    delete curr;

    return data;
}

////////////////////////////////////////////////////////////////////////////////
////////// auxilary function ///////////////////////////////////////////////////

template <class T>
int OrderedList<T>::SearchNodePosition(T *data_)
{
    ListNode<T> *curr = m_head;
    std::shared_lock<std::shared_mutex> read_lock(curr->m_rwmutex);
    T *tmp = data_;
    int position = 0;
    int index = nullptr == data_ ? 0 : 1;
    
    while (curr)
    {
        if (0 > m_cmp_func_(tmp, curr->val))
        {
            if (nullptr == data_)
            {
                tmp = curr->val;
            }
            position = index;

        }
        ++index;
        curr = curr->next;
        if (curr)
        {
            read_lock = std::shared_lock<std::shared_mutex>(curr->m_rwmutex);
        }
    }

    return position;
}

////////////////////////////////////////////////////////////////////////////////
////////// Geters  /////////////////////////////////////////////////////////////

template <class T>
sem_t *OrderedList<T>::GetSemaphore()
{
    return &m_sem;
}

template <class T>
ListNode<T> *OrderedList<T>::GetBegin()
{
    return m_head;
}

#endif /* __ORDEREDLIST_HPP__ */