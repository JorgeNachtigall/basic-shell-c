#include "server.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main()
{
    struct user user_list[5];
    char param1[15];
    char param2[15];
    char param3[15];
    int flag_break = 0;
    char holder[500];

    key_t key;
    int msgid;
    int msgid1;
    int msgid2;

    msgid1 = msgget(10002, 0777 | IPC_CREAT);

    printf("%d\n", msgid1);
    printf("%d\n", msgid2);

    make_all_users(user_list);

    int i = 0;
    int j = 0;

    int operation_type;

    while (1)
    {
        msgrcv(msgid1, &message, sizeof(message), 0, 0);
        printf("Data Received is : %s \n", message.mesg_text);

        while (message.mesg_text[i] != ' ')
        {
            if (message.mesg_text[i] == '\0')
            {
                flag_break = 1;
                j = 0;
                i = 0;
                break;
            }
            param1[j] = message.mesg_text[i];
            j++;
            i++;
        }

        if (flag_break != 1)
        {

            j = 0;
            i++;

            while (message.mesg_text[i] != ' ')
            {
                if (message.mesg_text[i] == '\0')
                {
                    flag_break = 1;
                    j = 0;
                    i = 0;
                    break;
                }
                param2[j] = message.mesg_text[i];
                j++;
                i++;
            }

            if (flag_break != 1)
            {

                j = 0;

                i++;

                if (message.mesg_text[i] != '\0')
                {
                    while (message.mesg_text[i] != '\0')
                    {
                        param3[j] = message.mesg_text[i];
                        j++;
                        i++;
                    }
                }

                j = 0;
                i = 0;
            }
        }
        if (strcmp(param1, "login") == 0)
        {
            if (login(param2, param3, user_list))
            {
                char id[10];
                sprintf(id, "%d", show_user_id(param2, user_list));
                callback(id);
            }
            else
            {
                callback("Usuário ou senha incorretos!");
            }
        }
        else if (strcmp(param1, "users") == 0)
        {
            callback(logged_users(user_list));
        }

        memset(param1, 0, sizeof(param1));
        memset(param2, 0, sizeof(param2));
        memset(param3, 0, sizeof(param3));
    }
    return 0;
}
