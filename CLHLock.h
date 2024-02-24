#ifndef CLHLOCK_H
#define CLHLOCK_H

#include "Lock.h"
#include <atomic>
#include <pthread.h>

struct QNode {
    bool locked;
    QNode() : locked(false) {}
};

class CLHLock : public Lock {
private:
    std::atomic<QNode*> tail;
    pthread_key_t myPredKey;
    pthread_key_t myNodeKey;

public:
    CLHLock();
    ~CLHLock();
    void lock() override;
    void unlock() override;

private:
    static void destroyQNode(void* ptr);
};

#endif // CLHLOCK_H
