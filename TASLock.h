#ifndef TASLOCK_H
#define TASLOCK_H

#include "Lock.h"
#include <atomic>

class TASLock : public Lock {
private:
    std::atomic<bool> state;

public:
    TASLock() : state(false) {}

    void lock() override;
    void unlock() override;
};

#endif // TASLOCK_H
