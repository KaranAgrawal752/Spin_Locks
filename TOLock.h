#ifndef TOLOCK_H
#define TOLOCK_H

#include "Lock.h"
#include <atomic>
#include <pthread.h>
#include <chrono>

class TimeUnit {
public:
    static constexpr long MILLISECONDS = 1;
};

class TOLock : public Lock {
private:
    struct QNode {
        QNode* pred = nullptr;
    };

    static QNode* const AVAILABLE;

    std::atomic<QNode*> tail;
    pthread_key_t myNodeKey;

public:
    TOLock();
    ~TOLock();
    bool tryLock(long time, TimeUnit unit);
    void unlock() override;

private:
    static void destroyQNode(void* ptr);
};

#endif // TOLOCK_H
