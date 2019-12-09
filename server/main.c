#include "server.c"
#include <stdio.h>

int main()
{
    struct user user_list[5];
    make_all_users(user_list);
    send_chat_message(0, 1, 222, "Funciona por favor2", user_list);
    send_chat_message(0, 1, 222, "Funciona por favor1", user_list);
    //static void receive_chat_message(int id, int from_id, int code, struct user *user_list);
    receive_chat_message(1, 0, 223, user_list);
    receive_chat_message(1, 0, 222, user_list);
    return 0;
}
