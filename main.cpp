#include "TASLock.h"
#include "TTASLock.h"
#include "BackoffLock.h"
#include "ALock.h"
#include "CLHLock.h"
#include "MCSLock.h"
#include "TOLock.h"
#include "Lock.h"
#include <iostream>
#include <vector>
#include <map>
#include <pthread.h> // Include pthread header
#include <unistd.h>  // for sleep
#include <mutex>
#include <chrono>
#include <fstream>


using namespace std;
std::mutex mtx;

void printAtomically(int start,int threadId, int sequence) {
    // Lock the mutex to ensure atomicity
    std::lock_guard<std::mutex> lock(mtx);
    
    // Print the line atomically
    std::cout << "Thread id: " << threadId << "  Seq in Consensus: " << sequence <<" Invoking time "<<start<< std::endl;
    // Mutex is automatically released when lock_guard goes out of scope
}
struct ThreadArgs {
    Lock* mutex;
    int id;
};

struct TOThreadArgs
{
    TOLock* mutex;
    int id;
};

void* threadFunction(void * args_ptr) {
    ThreadArgs* args = static_cast<ThreadArgs*>(args_ptr);

    // Accessing univ_obj and invoc_obj from args
    Lock* mutex = args->mutex;
    int id=args->id;
    mutex->lock();
    int c=10;
    while(c--){}
    mutex->unlock();

    return nullptr;
}
void* TOthreadFunction(void * args_ptr) {
    TOThreadArgs* args = static_cast<TOThreadArgs*>(args_ptr);

    // Accessing univ_obj and invoc_obj from args
    TOLock* mutex = args->mutex;
    int id=args->id;

    if(mutex->tryLock(1))
    {
        int c=10;
        while(c--){}
        mutex->unlock();
    }

    return nullptr;
}

int main() {
    cout<<"main started\n";
    vector<string> locks={"TASLock", "TTASLock", "BackoffLock", "ALock", "CLHLock", "MCSLock", "TOLock"};
    int N=100;
    vector<vector<int>>  time_res(7,vector<int>(N+1,0));
    for(int thread_count=1;thread_count<=N;thread_count++)
    {
        cout<<"6th lock with no of thread "<<thread_count<<endl;
        TOLock* mutex=new TOLock();
        auto start=chrono::high_resolution_clock::now();
        vector<pthread_t> myThread(thread_count);
        for (int i = 0; i < thread_count; i++) {
            TOThreadArgs* args = new TOThreadArgs{mutex,i};
            if (pthread_create(&myThread[i], nullptr, TOthreadFunction, args)) {
                cerr << "Error creating thread." << endl;
                return 1;
            }
            // cout<<i<<" ";
        }

        // Wait for all threads to finish execution
        for (int i = 0; i < thread_count; i++) {
            if (pthread_join(myThread[i], NULL)) {
                cerr << "Error joining thread." << endl;
                return 1;
            }
        }
        auto end=chrono::high_resolution_clock::now();
        auto duration=chrono::duration_cast<chrono::microseconds>(end - start);
        time_res[6][thread_count]=duration.count();
    }
    for(int i=5;i>=0;i--)
    {
        for(int thread_count=1;thread_count<=N;thread_count++)
        {
            cout<<i<<"th lock with no of thread "<<thread_count<<endl;
            Lock* mutex;
            switch(i)
            {
                case 0: mutex=new TASLock();break;
                case 1: mutex=new TTASLock();break;
                case 2: mutex=new BackoffLock();break;
                case 3: mutex=new ALock(thread_count);break;
                case 4: mutex=new CLHLock();break;
                case 5: mutex=new MCSLock();break;
            }
            auto start=chrono::high_resolution_clock::now();
            vector<pthread_t> myThread(thread_count);
            for (int i = 0; i < thread_count; i++) {
                ThreadArgs* args = new ThreadArgs{mutex,i};
                if (pthread_create(&myThread[i], nullptr, threadFunction, args)) {
                    cerr << "Error creating thread." << endl;
                    return 1;
                }
                // cout<<i<<" ";
            }

            // Wait for all threads to finish execution
            for (int i = 0; i < thread_count; i++) {
                if (pthread_join(myThread[i], NULL)) {
                    cerr << "Error joining thread." << endl;
                    return 1;
                }
            }
            // cout<<"ended **************************************"<<endl;
            auto end=chrono::high_resolution_clock::now();
            auto duration=chrono::duration_cast<chrono::microseconds>(end - start);
            time_res[i][thread_count]=duration.count();
        }
    }
    

    ofstream file("result");
    if (file.is_open()) {
        file<<"7 "<<N<<endl;
        for(int i=0;i<7;i++)
        {
            for(int j=1;j<=N;j++)
            {
                file<<1000.00/(double)time_res[i][j]<<" ";
            }
            file<<endl;
        }
        file.close();
    } else {
        cerr << "Unable to open file: " << "result" << endl;
    }
    return 0;
}
