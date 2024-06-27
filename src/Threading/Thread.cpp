/*
 * Project: Cedar-Core
 * Copyright (C) 2024 Cedar Community
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <Cedar/Core/Threading/Thread.h>
#include <Cedar/Core/Exceptions/RuntimeException.h>

#include <pthread.h>

using namespace Cedar::Core;
using namespace Cedar::Core::Threading;

struct Thread::Impl {
public:
    pthread_t thread;
    Function<void> func;
    Boolean started;

    static void* threadFunc(void* arg) {
        auto* impl = static_cast<Impl*>(arg);
        impl->func();
        return nullptr;
    }

    Impl(const Function<void>& f) : func(f), started(false) {}

    void start() {
        if (!started) {
            started = true;
            if (pthread_create(&thread, nullptr, threadFunc, this) != 0) {
                throw RuntimeException("Failed to create thread");
            }
        }
    }

    ~Impl() {
        if (started) {
            pthread_detach(thread);
        }
    }

    void join() {
        if (started) {
            pthread_join(thread, nullptr);
        }
    }

    void detach() {
        if (started) {
            pthread_detach(thread);
        }
    }
};

Thread::Thread(const Function<void>& func) : pImpl(Memory::makeShared<Impl>(func)) {}

Thread::~Thread() {}

Thread::Thread(Thread&& other) noexcept : pImpl(std::move(other.pImpl)) {}

Thread& Thread::operator=(Thread&& other) noexcept {
    if (this != &other) {
        pImpl = std::move(other.pImpl);
    }
    return *this;
}

void Thread::start() {
    pImpl->start();
}

void Thread::join() {
    pImpl->join();
}

void Thread::detach() {
    pImpl->detach();
}
