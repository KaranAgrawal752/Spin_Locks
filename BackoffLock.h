#ifndef BACKOFFLOCK_H
#define BACKOFFLOCK_H

#include "Lock.h"
#include "Backoff.h"
#include <atomic>

class BackoffLock : public Lock {
private:
    std::atomic<bool> state;
    static const int MIN_DELAY=1; /* Your min delay in milliseconds*/
    static const int MAX_DELAY=10; /* Your max delay in milliseconds*/

public:
    BackoffLock();
    void lock() override;
    void unlock() override;
};

#endif // BACKOFFLOCK_H
