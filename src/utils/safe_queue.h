#pragma once

#include <pthread.h>
#include <queue>

namespace utils
{
    template <typename T>
    class SafeQueue
    {
    public:
        SafeQueue(void);
        ~SafeQueue(void);
        void push(T t);
        T pop(void);
    private:
        std::queue<T> queue;
        pthread_mutex_t mutex;
        pthread_cond_t condition_variable;
    };
}
