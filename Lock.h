#ifndef LOCK_H
#define LOCK_H

class Lock {
public:
    virtual void lock() = 0;    // before entering critical section
    virtual void unlock() = 0;  // before leaving critical section
    virtual ~Lock() {}           // virtual destructor for proper polymorphic behavior
};

#endif // LOCK_H