#ifndef __ORDEREDLIST_HPP__
#define __ORDEREDLIST_HPP__
#include <mutex>        // std::mutex, std::lock_guard
#include <shared_mutex>        // std::mutex, std::lock_guard
#include <functional> // std::funciton<()>
//#include <condition_variable> //mutable std::condition_variable_any
#include <semaphore.h>
//#include <atomic>

template <class T>
struct ListNode {
    T &val;
    ListNode *next;
    ListNode() : val(), next(nullptr) {}
    ListNode(T &x) : val(x), next(nullptr) {}
    ListNode(T &x, ListNode *next) : val(x), next(next) {}
 };


template <class T>
class QuickPushOrderedList; 

template <class T>
class QuickPopOrderedList; 

template <class T>
class OrderedList
{
    public: 
        OrderedList() = delete;
        OrderedList(std::function<int(const T*, const T*)> cmp_func_);
        virtual ~OrderedList();
        virtual T &Pop();
        virtual void Push(T &data_);
      
        int SearchNodePosition(T *data_);
        ListNode<T> *AccessNodeAt(int position_);
        std::function<int(const T*, const T*)> GetComp();
        std::mutex *GetMutex();
        std::shared_mutex *GetSharedSearchMutex();
        std::shared_mutex *GetSharedInsertMutex();
        sem_t *GetSemaphore();
        ListNode<T> *Begin();
        void SetBegin(ListNode<T> *new_node_);
    private:
        std::function<int(const T*, const T*)> m_cmp_func_;
        mutable std::mutex m_mutex;
        mutable sem_t m_sem;
        ListNode<T> *m_head;
        mutable std::shared_mutex m_shared_search_mutex;
        mutable std::shared_mutex m_shared_insert_mutex;

};

////////// Implementations ////////////////////////////////////////////////////


template <class T>
OrderedList<T>::OrderedList(std::function<int(const T*, const T*)> cmp_func_) 
: m_cmp_func_(cmp_func_), m_head(nullptr)
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

template <class T>
T &OrderedList<T>::Pop()
{
    sem_wait(&m_sem);

    int position = SearchNodePosition(nullptr);

    std::unique_lock<std::shared_mutex> shared_lock1(m_shared_search_mutex);
    std::unique_lock<std::shared_mutex> shared_lock2(m_shared_insert_mutex);

    ListNode<T>* holding_ptr = m_head;
    ListNode<T>* prev = AccessNodeAt(position - 1);

    T *data = &holding_ptr->val;
    if (!prev)
    {
        m_head = m_head->next;
    }
    else
    {
        holding_ptr = prev->next;
        data = &holding_ptr->val;
        prev->next = holding_ptr->next;
    }

    delete holding_ptr;

    return *data;
}

template <class T>
void OrderedList<T>::Push(T &data_)
{
    ListNode<T>* new_node = new ListNode<T>(data_);
    int position = SearchNodePosition(&new_node->val);

    std::shared_lock<std::shared_mutex> shared_lock(m_shared_insert_mutex);    
    std::unique_lock<std::mutex> lock(m_mutex);

    ListNode<T>* prev = AccessNodeAt(position - 1);
    if (!prev)
    {
        new_node->next = m_head;
        m_head = new_node;
    }
    else
    {
        new_node->next = prev->next;
        prev->next = new_node;
    }

    sem_post(&m_sem);
}

////////// auxilary functions //////////////////////////////////////////////////

template <class T>
int OrderedList<T>::SearchNodePosition(T *data_)
{
    
    std::shared_lock<std::shared_mutex> lock(m_shared_search_mutex);
    ListNode<T> *curr = m_head;
    T *tmp = data_;
    int position = 0;
    int index = nullptr == data_ ? 0 : 1;
    
    while (curr)
    {
        if (0 > m_cmp_func_(tmp, &curr->val))
        {
            if (nullptr == data_)
            {
                tmp = &curr->val;
            }
            position = index;

        }
        ++index;
        curr = curr->next;
    }

    return position;
}

template <class T>
ListNode<T> *OrderedList<T>::AccessNodeAt(int position_)
{
    if (0 > position_)
    {
        return nullptr;
    }
    
    ListNode<T> *curr = m_head;
    for (int i = 0; i < position_; ++i)
    {
        #ifndef NDEBUG
        if (!curr)
        {
            throw std::runtime_error("Access position is out of bounds"); 
        }
        #endif
        curr = curr->next;
    }

    return curr;
}


////////// Geters and Seters ///////////////////////////////////////////////////


template <class T>
std::function<int(const T*, const T*)> OrderedList<T>::GetComp()
{
    return m_cmp_func_;
}
template <class T>
std::mutex *OrderedList<T>::GetMutex()
{
    return &m_mutex;
}
template <class T>
std::shared_mutex *OrderedList<T>::GetSharedSearchMutex()
{
    return &m_shared_search_mutex;
}
template <class T>
std::shared_mutex *OrderedList<T>::GetSharedInsertMutex()
{
    return &m_shared_insert_mutex;
}
template <class T>
sem_t *OrderedList<T>::GetSemaphore()
{
    return &m_sem;
}
template <class T>
ListNode<T> *OrderedList<T>::Begin()
{
    return m_head;
}
template <class T>
void OrderedList<T>::SetBegin(ListNode<T> *new_node_)
{
    m_head = new_node_;
}
#endif /* __ORDEREDLIST_HPP__ */