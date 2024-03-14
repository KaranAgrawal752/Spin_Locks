#include "CLHLock.h"

CLHLock::CLHLock() : tail(nullptr) {
    pthread_key_create(&myPredKey, destroyQNode);
    pthread_key_create(&myNodeKey, destroyQNode);
}

CLHLock::~CLHLock() {
    pthread_key_delete(myPredKey);
    pthread_key_delete(myNodeKey);
}

void CLHLock::lock() {
    std::atomic_thread_fence(std::memory_order_acquire);

    QNode* myNode = reinterpret_cast<QNode*>(pthread_getspecific(myNodeKey));
    if (!myNode) {
        myNode = new QNode();
        pthread_setspecific(myNodeKey, myNode);
    }
    myNode->locked = true;

    QNode* pred = tail.exchange(myNode, std::memory_order_acq_rel); // Acquire-release memory order
    pthread_setspecific(myPredKey, pred);

    while (pred && pred->locked) {
        std::atomic_thread_fence(std::memory_order_acq_rel);
    }
}

void CLHLock::unlock() {

    QNode* myNode = reinterpret_cast<QNode*>(pthread_getspecific(myNodeKey));

    myNode->locked = false;
    std::atomic_thread_fence(std::memory_order_acq_rel);

    pthread_setspecific(myNodeKey, reinterpret_cast<void*>(pthread_getspecific(myPredKey)));
}

void CLHLock::destroyQNode(void* ptr) {
    // delete reinterpret_cast<QNode*>(ptr);
}
