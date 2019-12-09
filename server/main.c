#include "server.c"
#include <stdio.h>

int main()
{
    struct user user_list[5];
    make_all_users(user_list);
    send_mail_message(0, 1, "Ola amigo", user_list);
    send_mail_message(0, 1, "Ola amigooooo", user_list);
    printf("%s\n", user_list[1].inbox->messages[0].message);
    printf("%s\n", user_list[1].inbox->messages[1].message);
    return 0;
}
