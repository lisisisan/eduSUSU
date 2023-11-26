#include <sys/mman.h>	// для создания отображенной в память области, в которую записывается сообщение
#include <sys/stat.h>	// для определения прав доступа к создаваемому объекту общей памяти
#include <fcntl.h>		// для записи сообщения
#include <unistd.h>		// для изменения размера отображенного в память файла
#include <string.h>		// для копирования сообщения в область, отображенную в память
#include <stdbool.h>	// для определения булевых значений
#include <semaphore.h>	// для синхронизации
#include <iostream>		// для вывода сообщений в консоль

int main()
{
    const int SIZE = 50;
    const char* name = "virtualMemoryObject";

    int fd = shm_open(name, O_CREAT | O_RDWR, 0666);  // создает именованный объект общей памяти
    ftruncate(fd, SIZE);
    // копируем из систеной тапблицы страниц в системную + добавляет строчки, конечно, чтобы записать
    unsigned char* dataWrite = (unsigned char*) mmap(           // создание виртуального адресного пространства, которое отображает содержимое файла, возварщает указатель на начало // адрес – номер страницы + смещение
        NULL,   // адрес начала отображаемой области памяти (NULL, для автоматического выбора адреса)
        SIZE,   // размер отображаемой области памяти в байтах 
        PROT_READ | PROT_WRITE,     // режимы доступа к отображенной области памяти (PROT_READ | PROT_WRITE)
        MAP_SHARED,     // флаги отображения памяти (MAP_SHARED)
        fd,     // файловый дескриптор(идентификации открытого файла) для объекта, который будет отображен в память (в данном случае - результат вызова shm_open)
        0);     // смещение объекта, начиная с которого будет отображена память (в данном случае - 0)

    const char* char_msg = "Hello! How are you?";
    memcpy(dataWrite, char_msg, SIZE); //копирует блок памяти размером SIZE из исходной области памяти, на которую указывает char_msg, в целевую область памяти, на которую указывает dataWrite.

    munmap(dataWrite, SIZE);        // используется для отключения(удаляем строчки из таблицы процесса) отображенной в памяти области и освобождения ресурсов, связанных с ней. сами таблицы не удаляются

    const char* writerName = "writer";
    sem_t* WriterEvent = sem_open(writerName, O_CREAT, 0666, 0);
    sem_post(WriterEvent);
    sem_close(WriterEvent);

	std::cout << "waiting for the reading to finish" << std::endl;

    while (true) {
        const char* readerName = "reader";
        sem_t* ReaderEvent = sem_open(readerName, O_CREAT, 0666, 0);
        if (ReaderEvent != NULL) {
			// sleep(1); // задержка на 1 секунду
            sem_wait(ReaderEvent);
            sem_close(ReaderEvent);
            sem_unlink(readerName);
            break;
        }
        sem_close(ReaderEvent);
    }

    const char* hMFileName = "virtualMemoryObject";
    shm_unlink(hMFileName);

    return 0;
}
