#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
#define BASE_PATH_PIPE "/tmp/MyPipe"

int main() {
    char message[7];     // буфер для записи сообщения от читателя [размер в байтах]
    int bytesRead = 0;
    int file;

    do {
        file = open(BASE_PATH_PIPE, O_RDONLY);    
    } while (file == -1);

    // sleep(1);

    for (int i = 0; i < 10; i++) {
        bytesRead = read(file,      // файловый дескриптор
        message,                    // буфер для записи
        8);  // 8 – максимальное количество байтов для чтения
        message[bytesRead] = '\0';  // для преобразования считанных байтов в строку 
        std::cout << message << std::endl;
        sleep(2);
    }

    close(file);
    return 0;
}
