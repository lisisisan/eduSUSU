#include <iostream>
#include <thread>
#include <mutex>
#include <cstdio>
#include <queue>

#define CXX_STANDARD 11

using namespace std;

const int BUFFER_SIZE = 10;

queue<int> buffer_queue;
mutex buffer_mutex;

void writer(int id) {
    int counter = 0;
    while (true) {
        int num;
        if (id == 1) {
            num = ++counter;
        } else {
            num = --counter;
        }


        while (buffer_queue.size() == BUFFER_SIZE) {
            this_thread::sleep_for(chrono::milliseconds(100 + id * 100));
        }
        

        unique_lock<mutex> lock(buffer_mutex);
        if (buffer_queue.size() < BUFFER_SIZE) {buffer_queue.push(num);}
        lock.unlock();
        
        printf("Writer %d wrote %d\n", id, num);
        
    }
}

void reader() {
    while (true) {
        int num;

        while (buffer_queue.empty()) {
            this_thread::sleep_for(chrono::milliseconds(rand() % 1000));
        }
        num = buffer_queue.front();
        buffer_queue.pop();

        printf("Reader read %d\n", num);

    }
}

int main() {
    thread writer1_thread(writer, 1);
    thread writer2_thread(writer, 2);
    thread reader_thread(reader);
    
    writer1_thread.join();
    writer2_thread.join();
    reader_thread.join();
    return 0;
}
