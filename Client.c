#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
  int server, portNumber;
  struct sockaddr_in servAdd; // server socket address

  if (argc != 3)
  {
    printf("Call model: %s <IP Address> <Port Number>\n", argv[0]);
    exit(0);
  }

  if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    fprintf(stderr, "Cannot create socket\n");
    exit(1);
  }

  servAdd.sin_family = AF_INET;
  sscanf(argv[2], "%d", &portNumber);
  servAdd.sin_port = htons((uint16_t)portNumber);

  if (inet_pton(AF_INET, argv[1], &servAdd.sin_addr) < 0)
  {
    fprintf(stderr, " inet_pton() has failed\n");
    exit(2);
  }

  if (connect(server, (struct sockaddr *)&servAdd, sizeof(servAdd)) < 0)
  {
    fprintf(stderr, "Failed to establish connection with server, exiting\n");
    exit(3);
  }

  while (1)
  {
    char message[1000];
    char messageFromServer[1000];
    int read_size;
    fprintf(stderr, "Type quit to quit, or type a command to execute in the server\n");
    fgets(message, 1000, stdin);
    if (strcmp(message, "quit\n") == 0)
    { 
      close(server);
      exit(0);
    }
    write(server, message, strlen(message) + 1);

    read_size = read(server, messageFromServer, sizeof(messageFromServer)); //read stdout from program
    fprintf(stderr, " Server's execution results: %s\n", messageFromServer);
    for (int i = 0; i < 1000; i++){
        messageFromServer[i] = 0;
    }
  }
}
