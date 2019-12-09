#include <stdio.h>

void main()
{
    char command[10] = "null";
    char command2[10] = "null";
    char command3[10] = "null";

    scanf("%s%s%s", command, command2, command3);

    printf("command: %s\n", command);
    printf("command2: %s", command2);
    printf("command3: %s", command2);
}