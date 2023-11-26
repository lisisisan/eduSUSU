#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
#define BASE_PATH_PIPE "/tmp/MyPipe"

int main() {
    const char* charList[] = { "hello_0", "hello_1", "hello_2", "hello_3", "hello_4",
                               "hello_5", "hello_6", "hello_7", "hello_8", "hello_9" };
    int file;

    mkfifo(BASE_PATH_PIPE, 0666);  // создаём именованный канал

    do {
        file = open(BASE_PATH_PIPE, O_WRONLY);  // открываем его, пока не откроем 
    } while (file == -1);

    for (int i = 0; i < 10; i++) {    // пишем массив charList и спим 2 сек
        write(         // системный вызов для записи данных в файловый дескриптор
            file,                 // файловый дескриптор, представляющий открытый канал
            charList[i],         //  указатель на массив символов, содержащий данные, которые нужно записать в канал
            strlen(charList[i]) + 1);    // длина строки charList[i], включая завершающий нулевой символ (+1 – маркер конца строки)
        sleep(2);
    }

    close(file);        // закрываем канал, теперь писать в него нельзя
    return 0;
}
