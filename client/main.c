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
    char command[10];
    int exit = 0;
    int my_id;

    printf("-> Insira seu nome de usuário:\n");
    scanf("%s", username);

    printf("-> Insira sua senha:\n");
    scanf("%s", password);

    login(username, password);
    sleep(1);
    msgrcv(msgid, &message, sizeof(message), 0, 0);

    memset(command, 0, sizeof(command));

    if (strcmp(message.mesg_text, "Usuário ou senha incorretos!") != 0)
    {
        my_id = atoi(message.mesg_text);
        while (exit == 0)
        {
            printf("$> ");
            scanf("%s", command);

            strcpy(message.mesg_text, command);
            msgsnd(msgid, &message, sizeof(message), 0);
            sleep(1);
            msgrcv(msgid, &message, sizeof(message), 0, 0);
            printf("%s", message.mesg_text);
        }
    }

    return 0;
}