#include "safe_queue.h"

namespace utils
{
    template<class T>
    SafeQueue<T>::SafeQueue(void)
    {
        pthread_mutex_init(&m_mutex, nullptr);
        pthread_cond_init(&m_condv, nullptr);
    }

    template<class T>
    SafeQueue<T>::~SafeQueue(void)
    {
        pthread_mutex_destroy(&m_mutex);
        pthread_cond_destroy(&m_condv);
    }

    template<class T>
    void SafeQueue<T>::push(T t)
    {
        pthread_mutex_lock(&mutex);
        queue.push(t);
        pthread_cond_signal(&condition_variable);
        pthread_mutex_unlock(&mutex);
    }

    template<class T>
    T SafeQueue<T>::pop(void)
    {
        pthread_mutex_lock(&mutex);
        while (queue.empty())
        {
            pthread_cond_wait(&condition_variable, &mutex);
        }
        T value = queue.front();
        queue.pop();
        pthread_mutex_unlock(&mutex);
        
        return value;
    }
}