#include "Backoff.h"
#include <unistd.h> // for usleep

Backoff::Backoff(int min, int max) : minDelay(min), maxDelay(max), limit(min) {
    std::random_device rd;
    randomGenerator.seed(rd());
}

void Backoff::backoff() {
    std::uniform_int_distribution<int> distribution(0, limit);
    int delay = distribution(randomGenerator);
    limit = std::min(maxDelay, 2 * limit);
    usleep(delay * 1000); // usleep takes microseconds, so converting milliseconds to microseconds
}
