#include "BackoffLock.h"

BackoffLock::BackoffLock() : state(false) {}

void BackoffLock::lock() {
    Backoff backoff(MIN_DELAY, MAX_DELAY);
    while (true) {
        while (state.load()) {};
        if (!state.exchange(true)) {
            return;
        } else {
            backoff.backoff();
        }
    }
}

void BackoffLock::unlock() {
    state.store(false);
}
