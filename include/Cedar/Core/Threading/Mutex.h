//
// Created by itshu on 18/6/2024.
//

#ifndef CEDAR_CORE_THREADING_MUTEX_H
#define CEDAR_CORE_THREADING_MUTEX_H

namespace Cedar::Core::Threading {
    class Mutex {
    public:
        Mutex();
        ~Mutex();

        void lock() const;
        void unlock() const;

    private:
        class Impl;
        Impl* pImpl;
    };
}

#endif //CEDAR_CORE_THREADING_MUTEX_H
