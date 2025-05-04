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
    socklen_t addrlen = sizeof(serv_addr);

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
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

    sendto(sock, "Player Connected", strlen("Player Connected"), 0, (struct sockaddr *)&serv_addr, addrlen);

    while (1) 
    {
        memset(buffer, 0, sizeof(buffer));
        input = recvfrom(sock, buffer, 1024, 0, (struct sockaddr *)&serv_addr, &addrlen);
        buffer[input] = '\0';
        printf("%s", buffer);

        if (strstr(buffer, "row")) 
        {
            char rc[10];
            int row, col;
            scanf("%d %d", &row, &col);
            snprintf(rc, sizeof(rc), "%d %d", row, col);
            sendto(sock, rc, strlen(rc), 0, (struct sockaddr *)&serv_addr, addrlen);
        } 
        else if (strstr(buffer, "Would")) 
        {
            char str[6];
            scanf("%s", str);
            sendto(sock, str, strlen(str), 0, (struct sockaddr *)&serv_addr, addrlen);
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
