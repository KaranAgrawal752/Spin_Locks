#include "TASLock.h"

void TASLock::lock() {
    while (state.exchange(true)) {}
}

void TASLock::unlock() {
    state.store(false);
}
