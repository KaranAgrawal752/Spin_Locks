#include "MCSLock.h"
#include <iostream>
using namespace std;

MCSLock::MCSLock() : tail(nullptr) {
    pthread_key_create(&myNodeKey, destroyQNode);
}

MCSLock::~MCSLock() {
    pthread_key_delete(myNodeKey);
}

void MCSLock::lock() {
    QNode* myNode = reinterpret_cast<QNode*>(pthread_getspecific(myNodeKey));
    if (!myNode) {
        myNode = new QNode();
        pthread_setspecific(myNodeKey, myNode);
    }

    std::atomic_thread_fence(std::memory_order_acq_rel);
    // Enqueue myself
    QNode* pred = tail.exchange(myNode, std::memory_order_acq_rel);
    if (pred != nullptr) {
        myNode->locked = true;
        std::atomic_thread_fence(std::memory_order_acq_rel);
        pred->next = myNode;

        // Wait until predecessor indicates that it's done
        while (myNode->locked) {
            std::atomic_thread_fence(std::memory_order_acq_rel);
        }
    }
}

void MCSLock::unlock() {

    QNode* myNode = reinterpret_cast<QNode*>(pthread_getspecific(myNodeKey));
    std::atomic_thread_fence(std::memory_order_acq_rel);
    // Check if there's a successor
    if (myNode->next == nullptr) {
        QNode* expected = myNode;
        if (tail.compare_exchange_strong(expected, nullptr, std::memory_order_acq_rel)) {
            return;
        }
        std::atomic_thread_fence(std::memory_order_acq_rel);
        // Wait until successor fills in the next field
        while (myNode->next == nullptr) {
            std::atomic_thread_fence(std::memory_order_acq_rel);
        }
    }


    // Notify successor
    myNode->next->locked = false;
    std::atomic_thread_fence(std::memory_order_acq_rel);
    myNode->next = nullptr;
}

void MCSLock::destroyQNode(void* ptr) {
    delete reinterpret_cast<QNode*>(ptr);
}
