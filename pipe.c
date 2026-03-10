#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int fd;
    char *fifo = "myfifo";
    char message[] = "Hello Ranjen";
    char buffer[100];
    pid_t pid;

    if (mkfifo(fifo, 0666) == -1) {
        perror("mkfifo");
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        fd = open(fifo, O_RDONLY);
        ssize_t n = read(fd, buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            printf("Child received: %s\n", buffer);
        }
        close(fd);
    } else {
        fd = open(fifo, O_WRONLY);
        write(fd, message, strlen(message) + 1);
        printf("Parent sent: %s\n", message);
        close(fd);
        wait(NULL);
        unlink(fifo);
    }

    return 0;
}
