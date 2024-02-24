#ifndef ALOCK_H
#define ALOCK_H

#include "Lock.h"
#include <pthread.h>
#include <vector>
#include <atomic>

class ALock : public Lock {
private:
    pthread_key_t mySlotIndex;
    std::atomic<int> tail;
    std::vector<std::atomic<bool>> flag;
    int size;

public:
    ALock(int capacity);
    ~ALock();
    void lock() override;
    void unlock() override;
};

#endif // ALOCK_H
