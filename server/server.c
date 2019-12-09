#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "server.h"

#define TOTAL_USERS 5
#define NO_CODE -999
#define DONT_EXIST -999
#define NOT_FOUND -404

struct user create_user(char name[], char password[]) // cria um usuário
{
    struct user new_user;
    strcpy(new_user.name, name);
    strcpy(new_user.password, password);
    new_user.status = 0;
    new_user.inbox = (struct mailbox *)malloc(sizeof(struct mailbox));
    //new_user.chat = (struct node *)malloc(sizeof(struct node));
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
        user_list[receiver].inbox->messages[new_message_index].from = sender;
        strcpy(user_list[receiver].inbox->messages[new_message_index].message, message);
    }
    else
    {
        user_list[receiver].inbox->total_messages = 1;
        user_list[receiver].inbox->messages = (struct mail *)malloc(user_list[receiver].inbox->total_messages * sizeof(struct mail));
        user_list[receiver].inbox->messages[new_message_index].from = sender;
        strcpy(user_list[receiver].inbox->messages[0].message, message);
    }
}

void show_all_mail(int id, struct user user_list[])
{
    int total_messages = user_list[id].inbox->total_messages;

    for (int i = 0; i < total_messages; i++)
    {
        int from = user_list[id].inbox->messages[i].from;
        printf("----\n");
        printf(">> De: %s\n", user_list[from].name);
        printf(">> Mensagem: %s\n", user_list[id].inbox->messages[i].message);
        printf("----\n");
    }
}

void send_chat_message(int sender, int receiver, int code, char message[], struct user *user_list)
{
    struct direct new_message;
    new_message.code = code;
    new_message.from = sender;
    strcpy(new_message.message, message);
    user_list[receiver].chat = enqueue(user_list[receiver].chat, new_message);
}

void receive_chat_message(int id, int from_id, int code, struct user *user_list)
{
    struct direct dequeued_message;
    dequeued_message = dequeue(user_list[id].chat, from_id, code);

    if (dequeued_message.code == NOT_FOUND)
    {
        printf("----\n");
        printf(">> Mensagem não encontrada.\n");
        printf("----\n");
        return;
    }

    if (dequeued_message.code != DONT_EXIST)
    {
        printf("----\n");
        printf(">> De: %s\n", user_list[dequeued_message.from].name);
        printf(">> Código: %d\n", dequeued_message.code);
        printf(">> Mensagem: %s\n", dequeued_message.message);
        printf("----\n");
    }
}

// implementação da fila

struct node *enqueue(struct node *head, struct direct message)
{
    struct node *new_node = malloc(sizeof(struct node));

    new_node->message = message;
    new_node->next = head;

    head = new_node;

    return head;
}

struct direct dequeue(struct node *head, int id, int code)
{
    struct node *current, *prev, *next_target = NULL;
    struct node *target = NULL;
    struct node *prev_target = NULL;
    struct direct retrieved_message;

    struct direct error_case;
    struct direct not_found;
    error_case.code = DONT_EXIST;
    not_found.code = NOT_FOUND;

    if (head == NULL)
    {
        printf(">> A fila de mensagens ainda não foi criada.\n");
        return error_case;
    }

    current = head;

    if (code == NO_CODE)
    {
        while (current->next != NULL)
        {
            if (current->message.from == id)
            {
                prev_target = prev;
                target = current;
                next_target = current->next;
            }
            prev = current;
            current = current->next;
        }

        if (current->message.from == id)
        {
            prev_target = prev;
            target = current;
            next_target = current->next;
        }
    }
    else
    {
        while (current->next != NULL)
        {
            if (current->message.from == id && current->message.code == code)
            {
                prev_target = prev;
                target = current;
                next_target = current->next;
            }
            prev = current;
            current = current->next;
        }

        if (current->message.from == id && current->message.code == code)
        {
            prev_target = prev;
            target = current;
            next_target = current->next;
        }
    }

    if (target == NULL)
        return not_found;

    if (next_target)
    {
        free(current);
        current = prev_target;
        current->next = next_target;
    }
    else
    {
        if (prev)
            prev->next = NULL;
        else
            head = NULL;
    }

    retrieved_message = target->message;

    return retrieved_message;
}
