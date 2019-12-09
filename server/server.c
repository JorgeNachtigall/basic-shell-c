#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "server.h"

#define TOTAL_USERS 5

struct user create_user(char name[], char password[]) // cria um usuário
{
    struct user new_user;
    strcpy(new_user.name, name);
    strcpy(new_user.password, password);
    new_user.status = 0;
    new_user.inbox = (struct mailbox *)malloc(sizeof(struct mailbox));
    return new_user;
}

void make_all_users(struct user *user_list) // cria todos os usuários do sistema
{
    user_list[0] = create_user("admin", "admin");
    user_list[1] = create_user("emillyn", "emillyn");
    user_list[2] = create_user("dhiully", "dhiully");
    user_list[3] = create_user("julia", "julia");
    user_list[4] = create_user("felipe", "felipe");
}

int login(char name[], char password[], struct user *user_list) // se usuário e senha corretos retorna 1
{
    for (int i = 0; i < TOTAL_USERS; i++)
    {
        if (strcmp(user_list[i].name, name) == 0 && strcmp(user_list[i].password, password) == 0)
        {
            user_list[i].status = 1;
            return 1;
        }
    }
    return 0;
}

void logged_users(struct user *userList)
{
    printf(">> Usuários logados:\n");
    for (int i = 0; i < TOTAL_USERS; i++)
    {
        if (userList[i].status == 1)
            printf("- %s\n", userList[i].name);
    }
}

void show_user_id(char name[], struct user *user_list)
{
    for (int i = 0; i < TOTAL_USERS; i++)
    {
        if (strcmp(user_list[i].name, name) == 0)
        {
            printf(">> Meu ID: %d\n", i);
        }
    }
}

void send_mail_message(int sender, int receiver, char message[], struct user *user_list)
{
    int new_message_index;

    if (user_list[receiver].inbox->total_messages > 0)
    {
        new_message_index = user_list[receiver].inbox->total_messages;
        user_list[receiver].inbox->total_messages++;
        user_list[receiver].inbox->messages = realloc(user_list[receiver].inbox->messages, user_list[receiver].inbox->total_messages * sizeof(struct mail));
        strcpy(user_list[receiver].inbox->messages[new_message_index].message, message);
    }
    else
    {
        user_list[receiver].inbox->total_messages = 1;
        user_list[receiver].inbox->messages = (struct mail *)malloc(user_list[receiver].inbox->total_messages * sizeof(struct mail));
        user_list[receiver].inbox->messages[new_message_index].from = sender;
        strcpy(user_list[receiver].inbox->messages[0].message, message);
    }

    //user_list[receiver].inbox->total_messages++;
}
