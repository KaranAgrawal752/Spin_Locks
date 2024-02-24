#ifndef BACKOFF_H
#define BACKOFF_H

#include <random>

class Backoff {
private:
    const int minDelay;
    const int maxDelay;
    int limit;
    std::mt19937 randomGenerator;

public:
    Backoff(int min, int max);
    void backoff();
};

#endif // BACKOFF_H
