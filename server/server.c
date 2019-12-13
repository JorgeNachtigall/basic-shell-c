#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "server.h"

#define TOTAL_USERS 5
#define NO_CODE -999
#define DONT_EXIST -999
#define NOT_FOUND -404
#define EXTERNAL_MQ_ID 10002

struct mesg_buffer message;

key_t key;
int msgid2;

struct user create_user(char name[], char password[]) // cria um usuário
{
    struct user new_user;
    strcpy(new_user.name, name);
    strcpy(new_user.password, password);
    new_user.status = 0;
    new_user.inbox = (struct mailbox *)malloc(sizeof(struct mailbox));
    new_user.chat = NULL;
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

int logout(int id, struct user *user_list)
{
    user_list[id].status = 0;
    return 0;
}

char *logged_users(struct user *userList)
{
    static char list[100] = "";
    memset(list, 0, sizeof(list));
    for (int i = 0; i < TOTAL_USERS; i++)
    {
        if (userList[i].status == 1)
        {
            strcat(list, "- ");
            strcat(list, userList[i].name);
            strcat(list, "\n");
        }
    }
    strcat(list, "\0");
    return list;
}

int show_user_id(char name[], struct user *user_list)
{
    for (int i = 0; i < TOTAL_USERS; i++)
    {
        if (strcmp(user_list[i].name, name) == 0)
        {
            return i;
        }
    }
    return 0;
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

char *show_all_mail(int id, struct user user_list[])
{
    static char mails[500] = "";
    memset(mails, 0, sizeof(mails));

    int total_messages = user_list[id].inbox->total_messages;

    for (int i = 0; i < total_messages; i++)
    {
        int from = user_list[id].inbox->messages[i].from;
        strcat(mails, "----\n>> De: ");
        strcat(mails, user_list[from].name);
        strcat(mails, "\n>> Mensagem: ");
        strcat(mails, user_list[id].inbox->messages[i].message);
        strcat(mails, "\n----\n");
    }
    strcat(mails, "\0");
    return mails;
}

char *send_chat_message(int sender, int receiver, int code, char message[], struct user *user_list)
{
    static char chat_return[50] = "";
    memset(chat_return, 0, sizeof(chat_return));
    struct direct new_message;
    new_message.code = code;
    new_message.from = sender;
    strcpy(new_message.message, message);
    if (user_list[receiver].status == 1)
    {
        user_list[receiver].chat = enqueue(user_list[receiver].chat, new_message);
        strcpy(chat_return, ">> Mensagem de chat enviada!\n");
        return chat_return;
    }
    else
    {
        strcpy(chat_return, ">> Mensagem não enviada, usuário offline!\n");
        return chat_return;
    }
}

char *receive_chat_message(int id, int from_id, int code, struct user *user_list)
{
    static char chat_get[500] = "";
    memset(chat_get, 0, sizeof(chat_get));
    struct direct dequeued_message;
    dequeued_message = dequeue(user_list[id].chat, from_id, code);

    if (dequeued_message.code == NOT_FOUND)
    {
        strcat(chat_get, "----\n>> Mensagem não encontrada.\n----\n");
        return chat_get;
    }

    if (dequeued_message.code != DONT_EXIST)
    {
        char code[10];

        sprintf(code, "%d", dequeued_message.code);

        strcat(chat_get, "----\n>> De: ");
        strcat(chat_get, user_list[dequeued_message.from].name);
        strcat(chat_get, "\n>> Código: ");
        strcat(chat_get, code);
        strcat(chat_get, "\n>> Mensagem: ");
        strcat(chat_get, dequeued_message.message);
        strcat(chat_get, "\n----\n");

        return chat_get;
    }

    strcpy(chat_get, ">> Fila de mensagens do usuário destino ainda não foi criada!\n");
    return chat_get;
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
    struct node *current = NULL;
    struct node *next_target = NULL;
    struct node *prev = NULL;
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

void callback(char callback_message[])
{
    key = ftok("progfile", 65);
    msgid2 = msgget(key, 0666 | IPC_CREAT);
    message.mesg_type = 1;
    strcpy(message.mesg_text, callback_message);
    msgsnd(msgid2, &message, sizeof(message), 0);
}