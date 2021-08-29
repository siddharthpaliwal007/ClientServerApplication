#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

void serviceClient(int);

int main(int argc, char *argv[])
{
  int sd, client, portNumber, status;
  struct sockaddr_in servAdd; // server socket address

  if (argc != 2)
  {
    printf("Call model: %s <Port Number>\n", argv[0]);
    exit(0);
  }

  sd = socket(AF_INET, SOCK_STREAM, 0);
  servAdd.sin_family = AF_INET;
  servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
  sscanf(argv[1], "%d", &portNumber);
  servAdd.sin_port = htons((uint16_t)portNumber);

  bind(sd, (struct sockaddr *)&servAdd, sizeof(servAdd));
  listen(sd, 5);

  while (1)
  {
    client = accept(sd, NULL, NULL);
    printf("Connection with a client established. Looking to execute commands\n");

    if (!fork())
      serviceClient(client);

    close(client);
  }
}

void serviceClient(int client)
{
  char bufferClientCommand[5000];
  while (1)
  { 
    if (dup2(client, 1) < 0)
    {
      printf("Unable to duplicate file descriptor.");
      exit(EXIT_FAILURE);
    }
    if (!read(client, bufferClientCommand, sizeof(bufferClientCommand)))
    {
      close(client);
      fprintf(stderr, "Bye, my client is dead, waiting for a new one\n");
      exit(0);
    }
    if (strcmp(bufferClientCommand, "quit\n") == 0)
    {
      close(client);
      exit(0);
    }
    else
    {
      fprintf(stderr, "%s", bufferClientCommand);
      system(bufferClientCommand);
    }
  }
}