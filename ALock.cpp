#include "ALock.h"
#include <atomic>
#include <iostream> // For debugging

ALock::ALock(int capacity) : size(capacity), tail(0), flag(capacity) {
    pthread_key_create(&mySlotIndex, nullptr);
    flag[0].store(true, std::memory_order_release); // Memory fence for initialization
}

ALock::~ALock() {
    pthread_key_delete(mySlotIndex);
}

void ALock::lock() {

    int slot = tail.fetch_add(1) % size;
    pthread_setspecific(mySlotIndex, reinterpret_cast<void*>(slot));

    std::atomic_thread_fence(std::memory_order_acq_rel);

    while (!flag[slot].load(std::memory_order_acq_rel)) {} // Memory fence for flag access
}

void ALock::unlock() {
    int slot = reinterpret_cast<long>(pthread_getspecific(mySlotIndex));
    flag[slot].store(false, std::memory_order_acq_rel); // Memory fence for flag modification
    flag[(slot + 1) % size].store(true, std::memory_order_acq_rel); // Memory fence for flag modification
}
