#ifndef TOLOCK_H
#define TOLOCK_H

#include <atomic>
#include <pthread.h>
#include <chrono>

class TOLock{
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
    bool tryLock(long time);
    void unlock() ;

private:
    static void destroyQNode(void* ptr);
};

#endif // TOLOCK_H
