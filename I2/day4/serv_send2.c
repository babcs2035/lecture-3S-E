#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("[server] Usage: %s <port>\n", argv[0]);
    exit(1);
  }

  int port = atoi(argv[1]);
  int ss = socket(PF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;
  if (bind(ss, (struct sockaddr *)&addr, sizeof(addr)) != 0)
  {
    printf("[server] bind failed\n");
    perror("bind");
    exit(1);
  }
  listen(ss, 16);
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  int s = accept(ss, (struct sockaddr *)&client_addr, &client_addr_len);
  if (s < 0)
  {
    printf("[server] accept failed\n");
    perror("accept");
    exit(1);
  }
  printf("[server] Connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

  FILE *fp;
  if ((fp = popen("rec -t raw -b 16 -c 1 -e s -r 44100 -", "r")) == NULL)
  {
    printf("[server] popen failed\n");
    perror("popen");
    exit(1);
  }
  printf("[server] Recording...\n");

  size_t buf_size = 1024;
  char buf[buf_size];
  while (1)
  {
    ssize_t r = read(fileno(fp), buf, buf_size);
    if (r == 0)
    {
      break;
    }
    if (r < 0)
    {
      printf("[server] read failed\n");
      perror("read");
      exit(1);
    }
    if (send(s, buf, r, 0) < 0)
    {
      printf("\n\n[server] send failed\n");
      perror("send");
      exit(1);
    }
  }

  shutdown(s, SHUT_WR);
  close(s);
  printf("[server] Disconnected\n");
}
