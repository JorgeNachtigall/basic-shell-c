#ifndef CLIENT_H
#define CLIENT_H

struct mesg_buffer
{
    long mesg_type;
    char mesg_text[500];
};

static void login(char name[], char password[]);

#endif
