#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Results function */
int results(const char *your_name, const char *opponent_name, char *your_move, char *opponent_move)
{
    /* Drum roll mechanism */
    sleep(1);
    fprintf(stdout, "\nThe results are in!\n");
    sleep(1);
    fprintf(stdout, "\n3...\n");
    sleep(1);
    fprintf(stdout, "2...\n");
    sleep(1);
    fprintf(stdout, "1...\n");
    sleep(1);
    fprintf(stdout, "\nPlayer %s chose %s.", your_name, your_move);
    fprintf(stdout, "\nPlayer %s chose %s.\n", opponent_name, opponent_move);
    sleep(1);
    

    /* Rock */
    if ((strcmp(your_move, "r") == 0) && (strcmp(opponent_move, "r") == 0))
    {
        fprintf(stdout, "\nIt's a tie!\n");
    }
    else if ((strcmp(your_move, "r") == 0) && (strcmp(opponent_move, "p") == 0))
    {
        fprintf(stdout, "\nYou lose!\n");
    }
    else if ((strcmp(your_move, "r") == 0) && (strcmp(opponent_move, "s") == 0))
    {
        fprintf(stdout, "\nYou win!\n");
    }

    /* Paper */
    else if ((strcmp(your_move, "p") == 0) && (strcmp(opponent_move, "r") == 0))
    {
        fprintf(stdout, "\nYou win!\n");
    }
    else if ((strcmp(your_move, "p") == 0) && (strcmp(opponent_move, "p") == 0))
    {
        fprintf(stdout, "\nIt's a tie!\n");
    }
    else if ((strcmp(your_move, "p") == 0) && (strcmp(opponent_move, "s") == 0))
    {
        fprintf(stdout, "\nYou lose!\n");
    }

    /* Scissors */
    else if ((strcmp(your_move, "s") == 0) && (strcmp(opponent_move, "r") == 0))
    {
        fprintf(stdout, "\nYou lose!\n");
    }
    else if ((strcmp(your_move, "s") == 0) && (strcmp(opponent_move, "p") == 0))
    {
        fprintf(stdout, "\nYou win!\n");
    }
    else if ((strcmp(your_move, "s") == 0) && (strcmp(opponent_move, "s") == 0))
    {
        fprintf(stdout, "\nIt's a tie!\n");
    }
    return 0;
}

/* Server function */
int server(const char *player, int port)
{
    int server_fd, client_fd, valread;
    struct sockaddr_in addr;
    int addr_len = sizeof(addr);
    char buffer[1024] = {0};
    char cli_name[1024] = {0};
    char cli_move[1024] = {0};
    char ser_move[1024] = {0};

    /* Game information */
    fprintf(stdout, "\nWelcome to RPS SERVER program!\n");
    fprintf(stdout, "\nPlayer name: %s\nPort number: %d\n", player, port);

    /* Creating server socket file descriptor */
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Server socket failed");
        exit(EXIT_FAILURE);
    }

    /* Binding socket to port */
    memset(&addr, 0, addr_len);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *) &addr, addr_len) < 0)
    {
        perror("Server socket binding failed");
        exit(EXIT_FAILURE);
    }

    /* Listen for client connection */
    if (listen(server_fd, 5) < 0)
    {
        perror("Server listening failed");
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "\nWaiting for client...\n");

    /* Accepting client connection */
    if ((client_fd = accept(server_fd, (struct sockaddr *) &addr, (socklen_t *) &addr_len)) < 0)
    {
        perror("Server accepting failed");
        exit(EXIT_FAILURE);
    }

    /* Reading initial from client */
    if (read(client_fd, buffer, 1024) < 0)
    {
        perror("Server reading failed");
        exit(EXIT_FAILURE);
    }
    
    fprintf(stdout, "\nPlayer %s just connected from client.\n", buffer);
    strcpy(cli_name, buffer);

    /* Sending initial message to client */
    if (send(client_fd, player, strlen(player), 0) < 0)
    {
        perror("Server sending failed");
        exit(EXIT_FAILURE);
    }

    /* Prompting server player for their move */
    memset(buffer, 0, sizeof(buffer));
    fprintf(stdout, "\nThe game starts now!\n\nr for rock\np for paper\ns for scissors\n");
    fprintf(stdout, "\nPlayer %s, please enter your move: ", player);
    scanf("%s", buffer);

    while (strcmp(buffer, "r") != 0 && strcmp(buffer, "p") != 0 && strcmp(buffer, "s") != 0)
    {
        fprintf(stdout, "\nInvalid move. Please try again.\n");
        fprintf(stdout, "\nPlayer %s, please enter your move: ", player);
        scanf("%s", buffer);
    }
    
    strcpy(ser_move, buffer);

    /* Sending player's move to client */
    if (send(client_fd, buffer, strlen(buffer), 0) < 0)
    {
        perror("Server sending failed");
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "\nWaiting for %s's move...\n", cli_name);

    /* Reading client player's move */
    memset(buffer, 0, sizeof(buffer));
    if (read(client_fd, buffer, 1024) < 0)
    {
        perror("Server reading failed");
        exit(EXIT_FAILURE);
    }
    strcpy(cli_move, buffer);
    fprintf(stdout, "\nPlayer %s has moved.\n", cli_name);

    /* Printing results */
    results(player, cli_name, ser_move, cli_move);

    /* Closing socket and shutting down server */
    close(client_fd);
    shutdown(server_fd, SHUT_RDWR);
    
    return 0;
}

/* Client function */
int client(const char *player, int port)
{
    int client_fd;
    struct sockaddr_in ser_addr;
    int ser_addr_len = sizeof(ser_addr);
    char buffer[1024] = {0};
    char ser_name[1024] = {0};
    char ser_move[1024] = {0};
    char cli_move[1024] = {0};

    /* Game information */
    fprintf(stdout, "\nWelcome to RPS CLIENT program!\n");
    fprintf(stdout, "\nPlayer name: %s\nPort number: %d\n", player, port);

    /* Creating client socket file descriptor */
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Client socket failed");
        exit(EXIT_FAILURE);
    }

    /* Connecting to server */
    memset(&ser_addr, 0, ser_addr_len);
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(port);

    if (connect(client_fd, (struct sockaddr *) &ser_addr, ser_addr_len) < 0)
    {
        perror("Client connection failed");
        exit(EXIT_FAILURE);
    }

    /* Sending initial message to server */
    if (send(client_fd, player, strlen(player), 0) < 0)
    {
        perror("Client sending failed");
        exit(EXIT_FAILURE);
    }

    /* Reading initial message from server */
    if (read(client_fd, buffer, 1024) < 0)
    {
        perror("Client reading failed");
        exit(EXIT_FAILURE);
    }
    strcpy(ser_name, buffer);

    fprintf(stdout, "\nPlayer %s just connected from server.\n", buffer);

    /* Prompting client player for their move */
    memset(buffer, 0, sizeof(buffer));
    fprintf(stdout, "\nThe game starts now!\n\nr for rock\np for paper\ns for scissors\n");
    fprintf(stdout, "\nPlayer %s, please enter your move: ", player);
    scanf("%s", buffer);

    while (strcmp(buffer, "r") != 0 && strcmp(buffer, "p") != 0 && strcmp(buffer, "s") != 0)
    {
        fprintf(stdout, "\nInvalid move. Please try again.\n");
        fprintf(stdout, "\nPlayer %s, please enter your move: ", player);
        scanf("%s", buffer);
    }
    
    strcpy(cli_move, buffer);

    /* Sending player's move to server */
    if (send(client_fd, buffer, strlen(buffer), 0) < 0)
    {
        perror("Client sending failed");
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "\nWaiting for %s's move...\n", buffer);

    /* Reading server player's move */
    memset(buffer, 0, sizeof(buffer));
    if (read(client_fd, buffer, 1024) < 0)
    {
        perror("Client reading failed");
        exit(EXIT_FAILURE);
    }
    strcpy(ser_move, buffer);
    fprintf(stdout, "\nPlayer %s has moved.\n", ser_name);

    /* Printing results */
    results(player, ser_name, cli_move, ser_move);

    /* Closing socket*/
    close(client_fd);

    return 0;
}

/* Main function */
int main(int argc, char const *argv[])
{
    /* Usage message and handling argument errors */
    if ((argc < 3) || (argc > 3))
    {
        fprintf(stderr, "Usage: ./[filename] [player] [port]\n");
        exit(EXIT_FAILURE);
    }

    const char *filename = argv[0];
    const char *player = argv[1];
    const char *port_str = argv[2];
    char *endptr;

    int port = (int) strtod(port_str, &endptr);

    if (port == 0)
    {
        fprintf(stderr, "User error: Wrong port number format.\n");
        exit(EXIT_FAILURE);
    }

    /* Checking filename to spawn program */
    if (strcmp(filename, "./rpsserver") == 0)
    {
        server(player, port);
    }
    
    if (strcmp(filename, "./rpsclient") == 0)
    {
        client(player, port);
    }
    
    return 0;
}

