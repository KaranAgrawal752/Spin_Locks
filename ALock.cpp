#include "ALock.h"

ALock::ALock(int capacity) : size(capacity), tail(0), flag(capacity) {
    pthread_key_create(&mySlotIndex, nullptr);
    flag[0].store(true);
}

ALock::~ALock() {
    pthread_key_delete(mySlotIndex);
}

void ALock::lock() {
    int slot = tail.fetch_add(1) % size;
    pthread_setspecific(mySlotIndex, reinterpret_cast<void*>(slot));
    while (!flag[slot].load()) {}
}

void ALock::unlock() {
    int slot = reinterpret_cast<long>(pthread_getspecific(mySlotIndex));
    flag[slot].store(false);
    flag[(slot + 1) % size].store(true);
}
