#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    printf("Usage: %s <ip> <port>\n", argv[0]);
    exit(1);
  }
  char *ip = argv[1];
  int port = atoi(argv[2]);

  int s = socket(PF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  inet_aton(ip, &server_addr.sin_addr);
  server_addr.sin_port = htons(port);
  if (connect(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
  {
    printf("[client] connect failed\n");
    perror("connect");
    exit(1);
  }
  printf("[client] Connected: %s:%d\n", inet_ntoa(server_addr.sin_addr), port);

  size_t buf_size = 1024;
  char buf[buf_size];
  while (1)
  {
    ssize_t r = recv(s, buf, buf_size, 0);
    if (r == 0)
    {
      break;
    }
    if (r < 0)
    {
      printf("[client] recv failed\n");
      perror("recv");
      exit(1);
    }
    write(1, buf, r);
  }

  close(s);
  printf("[client] Disconnected\n");
}
