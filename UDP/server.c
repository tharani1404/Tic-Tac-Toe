#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void start_game(char tictactoe[3][3]) 
{
    for (int i = 0; i < 3; i++) 
    {
        for (int j = 0; j < 3; j++) 
        {
            tictactoe[i][j] = '_';
        }
    }
}

int winner_found(char tictactoe[3][3]) 
{
    for (int i = 0; i < 3; i++) 
    {
        if (tictactoe[i][0] != '_' && tictactoe[i][0] == tictactoe[i][1] && tictactoe[i][1] == tictactoe[i][2]) 
        {
            return 1;
        }
        if (tictactoe[0][i] != '_' && tictactoe[0][i] == tictactoe[1][i] && tictactoe[1][i] == tictactoe[2][i]) 
        {
            return 1;
        }
    }
    if (tictactoe[0][0] != '_' && tictactoe[0][0] == tictactoe[1][1] && tictactoe[1][1] == tictactoe[2][2]) 
    {
        return 1;
    }
    if (tictactoe[0][2] != '_' && tictactoe[0][2] == tictactoe[1][1] && tictactoe[1][1] == tictactoe[2][0]) 
    {
        return 1;
    }
    return 0;
}

int game_draw(char tictactoe[3][3]) 
{
    for (int i = 0; i < 3; i++) 
    {
        for (int j = 0; j < 3; j++) 
        {
            if (tictactoe[i][j] == '_') 
            {
                return 0;
            }
        }
    }
    return 1;
}

void send_game(char tictactoe[3][3], int server, struct sockaddr_in *client1, struct sockaddr_in *client2, socklen_t addrlen) 
{
    char game_state[1024];
    snprintf(game_state, 1024, "Game Status:\n%c %c %c\n%c %c %c\n%c %c %c\n",tictactoe[0][0], tictactoe[0][1], tictactoe[0][2],tictactoe[1][0], tictactoe[1][1], tictactoe[1][2],tictactoe[2][0], tictactoe[2][1], tictactoe[2][2]);
    sendto(server, game_state, strlen(game_state), 0, (struct sockaddr *)client1, addrlen);
    sendto(server, game_state, strlen(game_state), 0, (struct sockaddr *)client2, addrlen);
}
void print_board(char tictactoe[3][3])
{
    for(int i=0;i<3;i++)
    {
        printf("%c %c %c\n",tictactoe[i][0],tictactoe[i][1],tictactoe[i][2]);
    }
    return;
}
int main() 
{
    int server;
    struct sockaddr_in address, client1, client2;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = {0};
    char tictactoe[3][3];
    start_game(tictactoe);

    if ((server = socket(AF_INET, SOCK_DGRAM, 0)) == 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(1234);

    if (bind(server, (struct sockaddr *)&address, sizeof(address)) < 0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for players...\n");

    recvfrom(server, buffer, 1024, 0, (struct sockaddr *)&client1, &addrlen);
    sendto(server, "Hello Player 1, You are 'X'\n", 29, 0, (struct sockaddr *)&client1, addrlen);

    recvfrom(server, buffer, 1024, 0, (struct sockaddr *)&client2, &addrlen);
    sendto(server, "Hello Player 2, You are 'O'\n", 29, 0, (struct sockaddr *)&client2, addrlen);

    printf("2 players are connected, Starting the Game...\n");

    int cplayer = 1; 
    char csmbl = 'X'; 
    int row, col;
    char str1[5], str2[5];

    while (1) 
    {
        memset(buffer, 0, sizeof(buffer));
        if (cplayer == 1) 
        {
            sendto(server, "It's Your turn.Enter (row column): ", 36, 0, (struct sockaddr *)&client1, addrlen);
            recvfrom(server, buffer, 1024, 0, (struct sockaddr *)&client1, &addrlen);
        } 
        else 
        {
            sendto(server, "It's Your turn,Enter (row column): ", 36, 0, (struct sockaddr *)&client2, addrlen);
            recvfrom(server, buffer, 1024, 0, (struct sockaddr *)&client2, &addrlen);
        }
        sscanf(buffer, "%d %d", &row, &col);
        row = row - 1; 
        col = col - 1;

        if (tictactoe[row][col] == '_') 
        {
            tictactoe[row][col] = csmbl;
            send_game(tictactoe, server, &client1, &client2, addrlen);

            if (winner_found(tictactoe)) 
            {
                printf("Final Game Status:\n");
                print_board(tictactoe);
                if (cplayer == 1) 
                {
                    printf("Player 1 Wins!!\n");
                    sendto(server, "Congrats! You Won...\n", 21, 0, (struct sockaddr *)&client1, addrlen);
                    sendto(server, "Better luck next time. Player 1 won the game..\n", 47, 0, (struct sockaddr *)&client2, addrlen);
                } 
                else 
                {
                    printf("Player 2 Wins!!\n");
                    sendto(server, "Congrats! You Won...\n", 21, 0, (struct sockaddr *)&client2, addrlen);
                    sendto(server, "Better luck next time. Player 2 won the game..\n", 47, 0, (struct sockaddr *)&client1, addrlen);
                }
                sendto(server, "Would you like to play another game?(yes/no)\n", 47, 0, (struct sockaddr *)&client1, addrlen);
                sendto(server, "Would you like to play another game?(yes/no)\n", 47, 0, (struct sockaddr *)&client2, addrlen);
                recvfrom(server, buffer, 1024, 0, (struct sockaddr *)&client1, &addrlen);
                sscanf(buffer, "%s", str1);
                recvfrom(server, buffer, 1024, 0, (struct sockaddr *)&client2, &addrlen);
                sscanf(buffer, "%s", str2);
                if(strcmp(str1,"yes") != 0 && strcmp(str2,"yes") != 0)
                {
                    sendto(server, "exit", 5, 0, (struct sockaddr *)&client1, addrlen);
                    sendto(server, "exit", 5, 0, (struct sockaddr *)&client2, addrlen);
                    close(server);
                    exit(0);
                } 
                else if(strcmp(str1,"yes")==0 && strcmp(str2,"yes")!=0)
                {
                    sendto(server, "Player 2 did not wish to play.exiting...\n", 42, 0, (struct sockaddr *)&client1, addrlen);
                    sendto(server, "exit", 5, 0, (struct sockaddr *)&client2, addrlen);
                    close(server);
                    exit(0);
                }
                else if(strcmp(str1,"yes")!=0 && strcmp(str2,"yes")==0)
                {
                    sendto(server, "Player 1 did not wish to play.exiting...\n", 42, 0, (struct sockaddr *)&client2, addrlen);
                    sendto(server, "exit", 5, 0, (struct sockaddr *)&client1, addrlen);
                    close(server);
                    exit(0);
                }
                else
                {
                    start_game(tictactoe);
                    memset(buffer, 0, sizeof(buffer));
                    cplayer = 2;  
                    csmbl = 'O';   
                    sendto(server,"New game started! You are 'X'.\n", 32, 0, (struct sockaddr *)&client1, addrlen);    
                    sendto(server,"New game started! You are 'O'.\n", 32, 0, (struct sockaddr *)&client2, addrlen);     
                    
                }
            }

            if (game_draw(tictactoe)) 
            {
                printf("Final Game Status:\n");
                print_board(tictactoe);
                printf("It's a Draw!!\n");
                sendto(server, "It's a Draw!\n", 13, 0, (struct sockaddr *)&client1, addrlen);
                sendto(server, "It's a Draw!\n", 13, 0, (struct sockaddr *)&client2, addrlen);
                sendto(server, "Would you like to play another game?(yes/no)\n", 47, 0, (struct sockaddr *)&client1, addrlen);
                sendto(server, "Would you like to play another game?(yes/no)\n", 47, 0, (struct sockaddr *)&client2, addrlen);
                recvfrom(server, buffer, 1024, 0, (struct sockaddr *)&client1, &addrlen);
                sscanf(buffer, "%s", str1);
                recvfrom(server, buffer, 1024, 0, (struct sockaddr *)&client2, &addrlen);
                sscanf(buffer, "%s", str2);
                if(strcmp(str1,"yes") != 0 && strcmp(str2,"yes") != 0)
                {
                    sendto(server, "exit", 5, 0, (struct sockaddr *)&client1, addrlen);
                    sendto(server, "exit", 5, 0, (struct sockaddr *)&client2, addrlen);
                    close(server);
                    exit(0);
                } 
                else if(strcmp(str1,"yes")==0 && strcmp(str2,"yes")!=0)
                {
                    sendto(server, "Player 2 did not wish to play.exiting...\n", 42, 0, (struct sockaddr *)&client1, addrlen);
                    sendto(server, "exit", 5, 0, (struct sockaddr *)&client2, addrlen);
                    close(server);
                    exit(0);
                }
                else if(strcmp(str1,"yes")!=0 && strcmp(str2,"yes")==0)
                {
                    sendto(server, "Player 1 did not wish to play.exiting...\n", 42, 0, (struct sockaddr *)&client2, addrlen);
                    sendto(server, "exit", 5, 0, (struct sockaddr *)&client1, addrlen);
                    close(server);
                    exit(0);
                }
                else
                {
                    start_game(tictactoe);
                    memset(buffer, 0, sizeof(buffer));
                    cplayer = 2;  
                    csmbl = 'O';   
                    sendto(server,"New game started! You are 'X'.\n", 32, 0, (struct sockaddr *)&client1, addrlen);    
                    sendto(server,"New game started! You are 'O'.\n", 32, 0, (struct sockaddr *)&client2, addrlen);     
                    
                }
            }

            cplayer = (cplayer == 1) ? 2 : 1; 
            csmbl = (csmbl == 'X') ? 'O' : 'X'; 
        } 
        else 
        {
            if (cplayer == 1) 
            {
                sendto(server, "Invalid move, Try Again\n", 25, 0, (struct sockaddr *)&client1, addrlen);
            } 
            else 
            {
                sendto(server, "Invalid move, Try Again\n", 25, 0, (struct sockaddr *)&client2, addrlen);
            }
        }
    }
    close(server);
    return 0;
}
