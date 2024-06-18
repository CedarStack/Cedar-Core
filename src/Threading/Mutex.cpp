//
// Created by itshu on 18/6/2024.
//

#include <Cedar/Core/Threading/Mutex.h>
#include <pthread.h>

using namespace Cedar::Core::Threading;

class Mutex::Impl {
public:
    Impl() {
        pthread_mutex_init(&mutex, nullptr);
    }

    ~Impl() {
        pthread_mutex_destroy(&mutex);
    }

    void lock() {
        pthread_mutex_lock(&mutex);
    }

    void unlock() {
        pthread_mutex_unlock(&mutex);
    }

private:
    pthread_mutex_t mutex;
};

Mutex::Mutex() : pImpl(new Impl()) {}

Mutex::~Mutex() {
    delete pImpl;
}

void Mutex::lock() const {
    pImpl->lock();
}

void Mutex::unlock() const {
    pImpl->unlock();
}
