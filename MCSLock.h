#ifndef MCSLOCK_H
#define MCSLOCK_H

#include "Lock.h"
#include <atomic>
#include <pthread.h>

class MCSLock : public Lock {
private:
    struct QNode {
        bool locked;
        QNode* next;
        QNode() : locked(false), next(nullptr) {}
    };

    std::atomic<QNode*> tail;
    pthread_key_t myNodeKey;

public:
    MCSLock();
    ~MCSLock();
    void lock() override;
    void unlock() override;

private:
    static void destroyQNode(void* ptr);
};

#endif // MCSLOCK_H
