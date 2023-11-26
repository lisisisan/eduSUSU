#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <iostream>

using namespace std;

int main()
{
    const int SIZE = 50;
    const char* name = "virtualMemoryObject";

    int fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, SIZE);

    while (true) {
        const char* writerName = "writer";
        sem_t* WriterEvent = sem_open(writerName, O_CREAT, 0666, 0);
        if (sem_wait(WriterEvent) == 0) {
            sem_close(WriterEvent);
            sem_unlink(writerName);
            break;
        }
        sem_close(WriterEvent);
    }

    unsigned char* dataWrite = (unsigned char*) mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    char* char_msg = new char[SIZE];
    memcpy(char_msg, dataWrite, SIZE);

    munmap(dataWrite, SIZE);

    cout << char_msg << endl;

    const char* readerName = "reader";
    sem_t* ReaderEvent = sem_open(readerName, O_CREAT, 0666, 0);
    sem_post(ReaderEvent);
    sem_close(ReaderEvent);

	delete[] char_msg;
    shm_unlink(name);

    return 0;
}


// g++ -std=c++11 -o Writer Writer.cpp -pthread