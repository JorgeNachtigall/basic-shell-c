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
    char param4[15];
    char param5[15];
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
        flag_break = 0;
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

                while (message.mesg_text[i] != ' ')
                {
                    if (message.mesg_text[i] == '\0')
                    {
                        flag_break = 1;
                        j = 0;
                        i = 0;
                        break;
                    }
                    param3[j] = message.mesg_text[i];
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
                        param4[j] = message.mesg_text[i];
                        j++;
                        i++;
                    }

                    j = 0;
                    i++;

                    if (flag_break != 1)
                    {
                        if (message.mesg_text[i] != '\0')
                        {
                            while (message.mesg_text[i] != '\0')
                            {
                                param5[j] = message.mesg_text[i];
                                j++;
                                i++;
                            }
                        }
                    }
                }
            }
        }
        j = 0;
        i = 0;

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
        else if (strcmp(param1, "mail") == 0)
        {
            //callback(logged_users(user_list));
            int sender = atoi(param4);
            int receiver = atoi(param2);
            send_mail_message(sender, receiver, param3, user_list);
            callback("Mensagem enviada!\n");
        }
        else if (strcmp(param1, "showmail") == 0)
        {
            //callback(logged_users(user_list));
            int id = atoi(param2);
            callback(show_all_mail(id, user_list));
        }
        else if (strcmp(param1, "send") == 0)
        {
            //callback(logged_users(user_list));
            int receiver = atoi(param2);
            int sender = atoi(param5);
            int code = atoi(param3);
            callback(send_chat_message(sender, receiver, code, param4, user_list));
        }
        else if (strcmp(param1, "receive") == 0)
        {
            int from = atoi(param2);
            int id, code;

            if (param4[0] != '\0')
            {
                code = atoi(param3);
                id = atoi(param4);
            }
            else
            {
                id = atoi(param3);
                code = -999;
            }

            callback(receive_chat_message(id, from, code, user_list));
        }
        else if (strcmp(param1, "exit") == 0)
        {
            int id = atoi(param2);
            int log = logout(id, user_list);
        }

        memset(param1, 0, sizeof(param1));
        memset(param2, 0, sizeof(param2));
        memset(param3, 0, sizeof(param3));
        memset(param4, 0, sizeof(param4));
        memset(param5, 0, sizeof(param5));
    }
    return 0;
}
