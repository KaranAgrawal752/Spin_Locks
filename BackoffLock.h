#ifndef BACKOFFLOCK_H
#define BACKOFFLOCK_H

#include "Lock.h"
#include "Backoff.h"
#include <atomic>

class BackoffLock : public Lock {
private:
    std::atomic<bool> state;
    static const int MIN_DELAY=1; /* Your min delay */
    static const int MAX_DELAY=100; /* Your max delay */

public:
    BackoffLock();
    void lock() override;
    void unlock() override;
};

#endif // BACKOFFLOCK_H
