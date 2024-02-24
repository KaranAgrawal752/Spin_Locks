#ifndef TTASLOCK_H
#define TTASLOCK_H

#include "Lock.h"
#include <atomic>

class TTASLock : public Lock {
private:
    std::atomic<bool> state;

public:
    TTASLock() : state(false) {}

    void lock() override;
    void unlock() override;
};

#endif // TTASLOCK_H
