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
void send_game(char tictactoe[3][3], int client1, int client2)
{
    char game_state[1024];
    snprintf(game_state, 1024, "Game Status:\n%c %c %c\n%c %c %c\n%c %c %c\n",tictactoe[0][0], tictactoe[0][1], tictactoe[0][2],tictactoe[1][0], tictactoe[1][1], tictactoe[1][2],tictactoe[2][0], tictactoe[2][1], tictactoe[2][2]);
    send(client1, game_state, strlen(game_state), 0);
    send(client2, game_state, strlen(game_state), 0);
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
    int client1, client2;
    int input;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char tictactoe[3][3];
    start_game(tictactoe);
    if ((server = socket(AF_INET, SOCK_STREAM, 0)) == 0)
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
    if (listen(server, 2) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Waiting for the players....\n");
    if ((client1 = accept(server, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    send(client1, "Hello Player 1, You are 'X'\n", 29, 0);
    if ((client2 = accept(server, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    send(client2, "Hello Player 2, You are 'O'\n", 29, 0);
    printf("2 players are connected, Starting the Game..\n");
    int cplayer = client1; 
    char csmbl = 'X'; 
    int row, col;
    char str1[5];
    char str2[5];
    while (1)
    {
        send(cplayer, "It's Your turn.Enter (row column): ", 36, 0);
        input = read(cplayer, buffer, 1024);
        buffer[input] = '\0';
        sscanf(buffer, "%d %d", &row, &col);
        row = row - 1; 
        col = col - 1;
        if (tictactoe[row][col] == '_')
        {
            tictactoe[row][col] = csmbl;
            send_game(tictactoe, client1, client2); 
            if (winner_found(tictactoe))
            {
                printf("Final Game Status:\n");
                print_board(tictactoe);
                if (cplayer == client1)
                {
                    printf("Player 1 Wins!!\n");
                    send(client1, "Congrats! You Won...\n", 21, 0);
                    send(client2, "Better luck next time. Player 1 won the game..\n", 47, 0);
                }
                else
                {
                    printf("Player 2 Wins!!\n");
                    send(client2, "Congrats! You Won...\n", 21, 0);
                    send(client1, "Better luck next time. Player 2 won the game..\n", 47, 0);
                }
                sleep(1);
                send(client1,"Would you like to play another game?(yes/no)", 45, 0);
                send(client2,"Would you like to play another game?(yes/no)", 45, 0);
                fflush(stdin);
                input = read(client1, buffer, 1024);
                buffer[input] = '\0';
                sscanf(buffer, "%s", str1);
                input = read(client2, buffer, 1024);
                buffer[input] = '\0';
                sscanf(buffer, "%s", str2);
                if(strcmp(str1,"yes") != 0 && strcmp(str2,"yes") != 0)
                {
                    send(client1, "exit", 5, 0);
                    send(client2, "exit", 5, 0);
                    close(client1);
                    close(client2);
                    close(server);
                    exit(0);
                }
                else if(strcmp(str1,"yes")==0 && strcmp(str2,"yes")!=0)
                {
                    send(client1,"Player 2 did not wish to play.exiting..\n",41,0);
                    send(client2, "exit", 5, 0);
                    close(client1);
                    close(client2);
                    close(server);
                    exit(0); 
                } 
                else if(strcmp(str1,"yes")!=0 && strcmp(str2,"yes")==0)
                {
                    send(client2,"Player 1 did not wish to play.exiting...\n",42,0);
                    send(client1, "exit", 5, 0);
                    close(client1);
                    close(client2);
                    close(server);
                    exit(0); 
                } 
                else
                {
                    start_game(tictactoe);
                    memset(buffer, 0, sizeof(buffer));
                    cplayer = client2;  
                    csmbl = 'O';        
                    send(client1, "New game started! You are 'X'.\n", 31, 0);
                    send(client2, "New game started! You are 'O'.\n", 31, 0);
                }
            }
            if (game_draw(tictactoe))
            {
                printf("Final Game Status:\n");
                print_board(tictactoe);
                printf("It's a Draw!\n");
                send(client1, "It's a Draw!\n", 13, 0);
                send(client2, "It's a Draw!\n", 13, 0);
                sleep(1);
                send(client1,"Would you like to play another game?(yes/no)", 45, 0);
                send(client2,"Would you like to play another game?(yes/no)", 45, 0);
                fflush(stdin);
                input = read(client1, buffer, 1024);
                buffer[input] = '\0';
                sscanf(buffer, "%s", str1);
                input = read(client2, buffer, 1024);
                buffer[input] = '\0';
                sscanf(buffer, "%s", str2);
                if(strcmp(str1,"yes") != 0 && strcmp(str2,"yes") != 0)
                {
                    send(client1, "exit", 5, 0);
                    send(client2, "exit", 5, 0);
                    close(client1);
                    close(client2);
                    close(server);
                    exit(0);
                }
                else if(strcmp(str1,"yes")==0 && strcmp(str2,"yes")!=0)
                {
                    send(client1,"Player 2 did not wish to play.exiting..\n",41,0);
                    send(client2, "exit", 5, 0);
                    close(client1);
                    close(client2);
                    close(server);
                    exit(0); 
                } 
                else if(strcmp(str1,"yes")!=0 && strcmp(str2,"yes")==0)
                {
                    send(client2,"Player 1 did not wish to play.exiting...\n",42,0);
                    send(client1, "exit", 5, 0);
                    close(client1);
                    close(client2);
                    close(server);
                    exit(0); 
                } 
                else
                {
                    start_game(tictactoe);
                    memset(buffer, 0, sizeof(buffer));  
                    cplayer = client2;  
                    csmbl = 'O';
                    send(client1, "New game started! You are 'X'.\n", 31, 0);
                    send(client2, "New game started! You are 'O'.\n", 31, 0);
                }
            }
            if (cplayer == client1)
            {
                cplayer = client2;
                csmbl = 'O';
            }
            else
            {
                cplayer = client1;
                csmbl = 'X';
            }
        }
        else
        {
            send(cplayer, "Invalid move, Try Again\n", 25, 0);
            sleep(0.5);
        }
    }
    close(client1);
    close(client2);
    close(server);
    return 0;
}
