#ifndef __ORDEREDLIST_HPP__
#define __ORDEREDLIST_HPP__
#include <shared_mutex>        // std::mutex, std::lock_guard
#include <functional> // std::funciton<()>
#include <semaphore.h>

typedef std::unique_lock<std::shared_mutex> WRITE_LOCK;
typedef std::shared_lock<std::shared_mutex> READ_LOCK;

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
* Note that m_head is a dummy node.
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
        void AccessNodeAt(int position, ListNode<T> **prev, ListNode<T> **curr, 
                          WRITE_LOCK *prev_lock, WRITE_LOCK *curr_lock);
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
* push a node with value data_.
*
* To ensure mutual exclusion, the locks of the current node and the
* previous nodes must be acquired for push to work. As soon as the
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
    WRITE_LOCK prev_lock(prev->m_rwmutex);
    ListNode<T> *curr = prev->next;
    WRITE_LOCK curr_lock;
    if (curr)
    {
        curr_lock = WRITE_LOCK(curr->m_rwmutex);
    }

    AccessNodeAt(position - 1, &prev, &curr, &prev_lock, &curr_lock);

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
    WRITE_LOCK prev_lock(prev->m_rwmutex);
    ListNode<T> *curr = prev->next;
    WRITE_LOCK curr_lock;
    if (curr)
    {
        curr_lock = WRITE_LOCK(curr->m_rwmutex);
    }

    AccessNodeAt(position - 1, &prev, &curr, &prev_lock, &curr_lock);

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
    READ_LOCK read_lock(curr->m_rwmutex);
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
            read_lock = READ_LOCK(curr->m_rwmutex);
        }
    }

    return position;
}

template <class T>
void OrderedList<T>::AccessNodeAt(int position, ListNode<T> **prev, 
                                  ListNode<T> **curr, WRITE_LOCK *prev_lock, 
                                  WRITE_LOCK *curr_lock)
{
    if (0 < position)
    {
        for (int i = 0; i < position && *curr != nullptr; ++i) 
        {
            *prev = *curr;
            *curr = (*curr)->next;
            (*prev_lock).swap(*curr_lock);
            if (*curr)
            {
                *curr_lock = WRITE_LOCK((*curr)->m_rwmutex);
            }
        }
    }      
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
