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
    fgets(username, 15, stdin);
    //scanf("%s", username);
    username[strcspn(username, "\n")] = '\0';

    printf("-> Insira sua senha:\n");
    fgets(password, 15, stdin);
    password[strcspn(password, "\n")] = '\0';
    //scanf("%s", password);

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
            char command[500];
            printf("$> ");
            fgets(command, 500, stdin);
            command[strcspn(command, "\n")] = ' ';
            //scanf("%[^\n]s", command);

            char id[10];
            sprintf(id, "%d", my_id);

            if (strcmp(command, "exit ") != 0)
            {
                strcat(command, id);
                strcpy(message.mesg_text, command);
                msgsnd(msgid, &message, sizeof(message), 0);
                msgrcv(msgid2, &message, sizeof(message), 0, 0);
                printf("%s", message.mesg_text);
                memset(command, 0, sizeof(command));
            }
            else
            {
                strcat(command, id);
                strcpy(message.mesg_text, command);
                msgsnd(msgid, &message, sizeof(message), 0);
                return 0;
            }
        }
    }

    return 0;
}