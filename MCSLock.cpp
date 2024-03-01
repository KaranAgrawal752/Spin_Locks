#include "MCSLock.h"
#include<iostream>
using namespace std;
MCSLock::MCSLock() : tail(nullptr) {
    cout<<"MCSLock constructor"<<endl;
    pthread_key_create(&myNodeKey, destroyQNode);
}

MCSLock::~MCSLock() {
    cout<<"MCSLock destructor"<<endl;
    pthread_key_delete(myNodeKey);
}

void MCSLock::lock() {
    QNode* myNode = reinterpret_cast<QNode*>(pthread_getspecific(myNodeKey));
    if (!myNode) {
        myNode = new QNode();
        pthread_setspecific(myNodeKey, myNode);
    }

    QNode* pred = tail.exchange(myNode);
    if (pred != nullptr) {
        myNode->locked = true;
        pred->next = myNode;

        // Wait until predecessor gives up the lock
        while (myNode->locked) { }
    }
}

void MCSLock::unlock() {
    QNode* myNode = reinterpret_cast<QNode*>(pthread_getspecific(myNodeKey));

    // If myNode->next is null, it means no other thread is waiting
    if (myNode->next == nullptr) {
        if (tail.compare_exchange_strong(myNode, nullptr)) {
            return;
        }
        // Wait until successor fills in the next field
        while (myNode->next == nullptr) {}
    }

    myNode->next->locked = false;
    myNode->next = nullptr;
}

void MCSLock::destroyQNode(void* ptr) {
    delete reinterpret_cast<QNode*>(ptr);
}
