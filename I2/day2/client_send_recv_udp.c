#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

  int s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  inet_aton(ip, &server_addr.sin_addr);
  server_addr.sin_port = htons(port);
  bind(s, (struct sockaddr *)&server_addr, sizeof(server_addr));

  size_t buf_size = 1000;
  char buf[buf_size + 1];
  memset(buf, 0, buf_size + 1);
  for (int _ = 0; _ < 50; ++_)
  {
    ssize_t r = read(0, buf, buf_size);
    if (r == 0)
    {
      break;
    }
    if (r < 0)
    {
      perror("read");
      exit(1);
    }
    if (sendto(s, buf, r, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
      perror("[client] sendto failed");
      exit(1);
    }
  }

  while (1)
  {
    ssize_t r = recvfrom(s, buf, buf_size, 0, NULL, NULL);
    if (r < 0)
    {
      perror("[client] recvfrom failed");
      exit(1);
    }
    if (r == buf_size)
    {
      // check if buf is all 1
      int flag = 1;
      for (int i = 0; i < buf_size; i++)
      {
        if (buf[i] != '1')
        {
          flag = 0;
          break;
        }
      }
      if (flag)
      {
        printf("[client] EOD detected\n");
        break;
      }
    }
    write(1, buf, r);
  }

  close(s);
  printf("[client] Disconnected\n");
}
