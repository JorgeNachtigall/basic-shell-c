#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "client.h"

#define EXTERNAL_MQ_ID 10002

struct mesg_buffer message;

key_t key;
int msgid;
int msgid2;

void login(char name[], char password[])
{

    msgid = msgget(10002, 0777 | IPC_CREAT);
    message.mesg_type = 1;

    strcpy(message.mesg_text, "login ");
    strcat(message.mesg_text, name);
    strcat(message.mesg_text, " ");
    strcat(message.mesg_text, password);
    msgsnd(msgid, &message, sizeof(message), 0);
}