#include "server.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define EXTERNAL_MQ_ID 10002

key_t key; /* key to be passed to msgget() */
int mqid;  /* return value from msgget() */

char msgbuf[100];
int msgsize = 0;

int main()
{
    struct user user_list[5];

    key = EXTERNAL_MQ_ID;
    mqid = msgget(key, 0777);

    make_all_users(user_list);

    while (1)
    {
        printf(msgbuf);
        msgsize = msgrcv(mqid, msgbuf, sizeof(msgbuf), 0, 0);
        if (msgsize > 0)
        {
            printf("%s", msgbuf);
        }
    }
    return 0;
}
