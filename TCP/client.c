#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() 
{
    int sock = 0, input;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        printf("Socket creation error\n");
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(1234);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) 
    {
        printf("Invalid address/Address not supported\n");
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    {
        printf("Connection Failed\n");
        return -1;
    }
    while (1)
    {
        memset(buffer, 0, sizeof(buffer));  
        input = read(sock, buffer, 1024);
        buffer[input] = '\0';
        printf("%s", buffer);
        if (strstr(buffer, "row")) 
        {
            char rc[10];
            int row;
            int col;
            scanf("%d",&row);
            scanf("%d",&col);
            snprintf(rc, sizeof(rc), "%d %d", row, col);
            send(sock, rc, strlen(rc), 0);
        }
        if (strncmp(buffer, "Would", 5) == 0)
        {
            char str[5];
            scanf("%s", str);
            send(sock, str, strlen(str), 0);
        }
        if(strcmp(buffer,"exit")==0)
        {
        printf("Server closed. Exiting...\n");
        break;
        }
        if(strstr(buffer,"exiting")!=NULL)
        {
            printf("Server closed. Exiting...\n");
            break;
        }
    }
    close(sock);
    return 0;
}
