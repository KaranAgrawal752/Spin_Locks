#include "TOLock.h"

// Define the static member variable
TOLock::QNode* const TOLock::AVAILABLE = nullptr;

TOLock::TOLock() : tail(nullptr) {
    pthread_key_create(&myNodeKey, destroyQNode);
}

TOLock::~TOLock() {
    pthread_key_delete(myNodeKey);
}

bool TOLock::tryLock(long time) {  //time in seconds
    auto startTime = std::chrono::steady_clock::now();
    auto patience = std::chrono::milliseconds(time * 1000);
    
    QNode* qnode = new QNode();
    pthread_setspecific(myNodeKey, qnode);
    qnode->pred = nullptr;
    QNode* myPred = tail.exchange(qnode);
    
    if (myPred == nullptr || myPred->pred == AVAILABLE) {
        return true;
    }
    
    while (std::chrono::steady_clock::now() - startTime < patience) {
        QNode* predPred = myPred->pred;
        if (predPred == AVAILABLE) {
            return true;
        } else if (predPred != nullptr) {
            myPred = predPred;
        }
    }
    
    if (!tail.compare_exchange_strong(qnode, myPred)) {
        qnode->pred = myPred;
    }
    
    return false;
}

void TOLock::unlock() {
    QNode* qnode = reinterpret_cast<QNode*>(pthread_getspecific(myNodeKey));
    
    if (!tail.compare_exchange_strong(qnode, nullptr)) {
        qnode->pred = AVAILABLE;
    }
}

void TOLock::destroyQNode(void* ptr) {
    delete reinterpret_cast<QNode*>(ptr);
}
