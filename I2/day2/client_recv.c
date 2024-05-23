#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

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
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  inet_aton(ip, &addr.sin_addr);
  addr.sin_port = htons(port);
  if (connect(s, (struct sockaddr *)&addr, sizeof(addr)) != 0)
  {
    perror("connect");
    exit(1);
  }

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
      perror("recv");
      exit(1);
    }
    write(1, buf, r);
  }

  close(s);
}
