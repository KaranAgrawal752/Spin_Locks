#include "TTASLock.h"

void TTASLock::lock() {
    while (true) {
        while (state.load()) {}; // Spin until lock is released

        if (!state.exchange(true))
            return;
    }
}

void TTASLock::unlock() {
    state.store(false);
}
