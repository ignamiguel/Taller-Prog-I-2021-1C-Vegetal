#pragma once

#include <pthread.h>

namespace utils
{
    class Thread
    {
    public:
        Thread();
        virtual ~Thread();
        void start(void);
        void join(void);
        virtual void* run(void) = 0;
    private:
        pthread_t thread;
        bool is_running;
    };
}