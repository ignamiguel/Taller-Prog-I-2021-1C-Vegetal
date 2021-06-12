#include "thread.h"
#include <stdexcept>

namespace utils
{
    // Helper function
    static void* runner(void* thread)
    {
        return static_cast<Thread *>(thread)->run();
    }

    Thread::Thread() : thread(0), is_running(false)
    {
    }

    Thread::~Thread()
    {
        if (is_running)
        {
            pthread_detach(thread);
            pthread_cancel(thread);
        }
    }

    void Thread::start(void)
    {
        is_running = pthread_create(&thread, nullptr, runner, this) == 0;
        if (!is_running)
        {
            throw std::runtime_error("ERROR: Unable to start thread");
        }
    }
    
    void Thread::join(void)
    {
        if (!is_running)
        {
            throw std::runtime_error("ERROR: Unable to join thread before it is started");
        }

        if (pthread_join(thread, nullptr))
        {
            throw std::runtime_error("ERROR: Unable to join thread");
        }
    }
}