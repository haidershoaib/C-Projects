#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_LEN 1024

#define FIFO_NAME "myfifo"

int main(void)
{
    char buf[256];
    int num, fd, nbytes;
    int tmp = 0;
    pid_t pid;
    char line[BUFFER_LEN];  //get command line
    char* argv[100];        //user command
    char* path= "/bin/";    //set path at bin
    char filepath[80];      //full file path
    ssize_t f;
    char op[32] = "";

    if (mkfifo(FIFO_NAME, 0666) < 0)
        perror("mkfifo");


while(1) {
    printf("--SHELL--214823660>> ");  //print shell prompt
    tmp = tmp + 1;

    if (!fgets(line, BUFFER_LEN, stdin)) {
        break;
    }

    size_t length = strlen(line);
    if (line[length - 1] == '\n') {
        line[length - 1] = '\0';
    }

    char *p, *s;
    p = strtok(line, " ");
    s = p;
    p = strtok(NULL, " ");


    if (strcmp(line, "exit") == 0) {            //check if command is exit
        break;
    }

    char *token;                  //split command into separate strings
    int i = 0;
    token = line;

    while (token != NULL) {
        argv[i] = token;
        token = strtok(NULL, " ");
        i++;
    }

    argv[i] = NULL;                     //set last value to NULL for execvp

    strcpy(filepath, path);           //copy /bin/ to file path
    strcat(filepath, argv[0]);            //add program to path

    for (i = 0; i < strlen(filepath); i++) {    //delete newline
        if (filepath[i] == '\n') {
            filepath[i] = '\0';
        }
    }

    if (strcmp(s, "FG") == 0) {


        pid = fork();
        if (pid == 0) {
            // printf(s);
            if ((fd = open(FIFO_NAME, O_WRONLY)) < 0)
                perror("child can't open");

            printf("….working on request…..\n");
            if ((num = write(fd, p, strlen(p))) < 0) {
                perror("child can't write");
            }

            close(fd);
            exit(0);
        }

        else {
            if ((fd = open(FIFO_NAME, O_RDONLY)) < 0)
                perror("parent can't open");

            if ((num = read(fd, buf, sizeof(buf))) < 0)
                perror("parent can't read");
            else {
                nbytes = read(fd, buf, sizeof(buf));

                while (strcmp(op, "Y") != 0 || strcmp(op, "N") != 0) {
                    const char msg[] = "….output is ready. Display it now [Y/N]>>";
                    write(0, msg, sizeof(msg) - 1);
                    f = read(0, op, sizeof op - 1);
                    op[f - 1] = '\0';

                    if (strcmp(op, "Y") == 0) {
                        //execvp(buf, argv);
                        system(buf);
                        break;
                    } else if (strcmp(op, "N") == 0) {
                        break;
                    }
                }
            }
            tmp = 0;
        }
    }

    else if(strcmp(s, "BG") == 0) {
        printf("….request submitted, returning prompt…..\n");

        if (tmp > 1) {
            pid = fork();
            if (pid == 0) {
                // printf(s);
                if ((fd = open(FIFO_NAME, O_WRONLY)) < 0)
                    perror("child can't open");


                if ((num = write(fd, p, strlen(p))) < 0) {
                    perror("child can't write");
                }

                close(fd);
                exit(0);
            } else {
                if ((fd = open(FIFO_NAME, O_RDONLY)) < 0)
                    perror("parent can't open");


                if ((num = read(fd, buf, sizeof(buf))) < 0)
                    perror("parent can't read");
                else {
                    int status;
                    pid = wait(&status);
                    nbytes = read(fd, buf, sizeof(buf));

                    while (strcmp(op, "Y") != 0 || strcmp(op, "N") != 0) {
                        const char msg[] = "….output for “BG ls” is ready. Display it now [Y/N]>>";
                        write(0, msg, sizeof(msg) - 1);
                        f = read(0, op, sizeof op - 1);
                        op[f - 1] = '\0';

                        if (strcmp(op, "Y") == 0) {
                            // execvp(buf, argv);
                            system(buf);
                            break;
                        } else if (strcmp(op, "N") == 0) {
                            break;
                        }
                    }
                }
            }
            tmp = 0;
        }
    }
}
    unlink(FIFO_NAME);
    return 0;
}