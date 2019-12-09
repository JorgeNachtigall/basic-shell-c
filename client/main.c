#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "client.c"

#define EXTERNAL_MQ_ID 10002

key_t key; /* key to be passed to msgget() */
int mqid;  /* return value from msgget() */

char msgbuf[100];
int msgsize = 0;

int main()
{
    int exit = 0;
    char username[15] = "admin";
    char password[15] = "admin";
    char command[10];

    printf("-> Insira seu nome de usuário:\n");
    scanf("%s", username);
    printf("-> Insira sua senha:\n");
    scanf("%s", password);

    //login(username, password);

    if (mqid == -1)
    {
        printf("msgget falhou\n");
        return 0;
    }

    strcpy(msgbuf, "login");
    /*strcat(msgbuf, " ");
    strcat(msgbuf, username);
    strcat(msgbuf, " ");
    strcat(msgbuf, password);*/

    int test;

    test = msgsnd(mqid, msgbuf, strlen(msgbuf) + 1, 0);

    printf("%s", msgbuf);

    /*
    while (!exit)
    {
        printf("$> \n");
        scanf("%s");
    }

    
    int x = 2;

    switch (x)
    {
    case 1:
        break;

    case 2:
        break;

    default:
        x = 5;
    }
    return 0;
    */
    return 0;
}