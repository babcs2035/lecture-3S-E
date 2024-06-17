#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
  int s;
  if (argc == 2)
  {
    // server mode
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
    s = accept(ss, (struct sockaddr *)&client_addr, &client_addr_len);
    if (s < 0)
    {
      printf("[server] Accept at %s:%d failed\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
      perror("accept");
      exit(1);
    }
    printf("[server] Connected to %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
  }
  else
  {
    printf("[server] Usage: %s <port>\n", argv[0]);
    exit(1);
  }

  FILE *fp_r;
  if ((fp_r = popen("rec -t raw -b 16 -c 1 -e s -r 44100 -", "r")) == NULL)
  {
    perror("[server] popen (rec) failed");
    exit(1);
  }
  printf("[server] Recording...\n");

  FILE *fp_w;
  if ((fp_w = popen("play -t raw -b 16 -c 1 -e s -r 44100 -", "w")) == NULL)
  {
    perror("[server] popen (play) failed");
    exit(1);
  }
  printf("[server] Playing...\n");

  size_t buf_size = 1 << 12;
  char buf[buf_size];
  while (1)
  {
    ssize_t r;

    // send
    r = read(fileno(fp_r), buf, buf_size);
    if (r == 0)
    {
      printf("\n[server] read ended\n");
      break;
    }
    if (r < 0)
    {
      perror("\n[server] read failed");
      break;
    }
    if (send(s, buf, r, 0) < 0)
    {
      perror("\n[server] send failed");
      break;
    }

    // receive
    r = read(s, buf, buf_size);
    if (r == 0)
    {
      printf("\n[server] recv ended\n");
      break;
    }
    if (r < 0)
    {
      perror("\n[server] recv failed");
      exit(1);
    }
    write(fileno(fp_w), buf, r);
  }

  shutdown(s, SHUT_WR);
  close(s);
  pclose(fp_r);
  pclose(fp_w);
  printf("\n[server] Disconnected\n");
}
