#ifndef SERVER_H
#define SERVER_H

struct user
{
    char name[8];
    char password[8];
    int status;            //deslogado = 0 | logado = 1
    struct mailbox *inbox; // simula uma caixa de entrada de email
    struct node *chat;     // simula um sistema de chat mais direto
};

struct mailbox
{
    struct mail *messages;
    int total_messages;
};

// como existem dois tipos de comunicação entre os usuários, foram criadas 2 structs
// a struct mail lida com as mensagens moldadas como emails
// a struct direct lida com mensagens moldadas como mensagens diretas de chat

struct mail
{
    int from;
    char message[50];
};

struct direct
{
    int from;
    int code;
    char message[25];
};

struct node
{
    struct direct message;
    struct node *next;
};

static struct node *enqueue(struct node *head, struct direct message);

static struct direct dequeue(struct node *head, int id, int code);

static struct user create_user(char name[], char password[]);

static void make_all_users(struct user userList[]);

static int login(char name[], char password[], struct user userList[]);

static int logout(int id, struct user *user_list);

static void logged_users(struct user userList[]);

static void show_user_id(char name[], struct user user_list[]);

static void send_mail_message(int sender, int receiver, char message[], struct user user_list[]);

static void show_all_mail(int id, struct user user_list[]);

static void send_chat_message(int sender, int receiver, int code, char message[], struct user *user_list);

static void receive_chat_message(int id, int from_id, int code, struct user *user_list);

#endif
