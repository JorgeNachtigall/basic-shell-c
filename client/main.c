#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "client.c"

int main()
{

    char username[15] = "admin";
    char password[15] = "admin";
    int exit = 0;
    int my_id;

    printf("-> Insira seu nome de usuário:\n");
    scanf("%s", username);

    printf("-> Insira sua senha:\n");
    scanf("%s", password);

    login(username, password);

    key = ftok("progfile", 65);
    msgid2 = msgget(key, 0666 | IPC_CREAT);

    msgid = msgget(10002, 0777 | IPC_CREAT);
    message.mesg_type = 1;

    msgrcv(msgid2, &message, sizeof(message), 0, 0);

    if (strcmp(message.mesg_text, "Usuário ou senha incorretos!") != 0)
    {
        my_id = atoi(message.mesg_text);
        while (exit == 0)
        {
            msgid2 = msgget(key, 0666 | IPC_CREAT);
            msgid = msgget(10002, 0777 | IPC_CREAT);
            char command[10] = "users";
            printf("$> ");
            scanf("%s", command);

            strcpy(message.mesg_text, command);
            msgsnd(msgid, &message, sizeof(message), 0);
            msgrcv(msgid2, &message, sizeof(message), 0, 0);
            printf("%s", message.mesg_text);

            memset(command, 0, sizeof(command));
        }
    }

    return 0;
}