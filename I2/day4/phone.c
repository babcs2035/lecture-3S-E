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
  else if (argc == 3)
  {
    // client mode
    char *ip = argv[1];
    int port = atoi(argv[2]);

    s = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    inet_aton(ip, &server_addr.sin_addr);
    server_addr.sin_port = htons(port);
    if (connect(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
    {
      printf("[client] Connect to %s:%d failed\n", inet_ntoa(server_addr.sin_addr), port);
      perror("connect");
      exit(1);
    }
    printf("[client] Connected to %s:%d\n", inet_ntoa(server_addr.sin_addr), port);
  }
  else
  {
    printf("[server] Usage: %s <port>\n", argv[0]);
    printf("[client] Usage: %s <address> <port>\n", argv[0]);
    exit(1);
  }
  int mode_num = argc == 2 ? 0 : 1;
  char *mode_str = mode_num == 0 ? "server" : "client";

  FILE *fp_r;
  if ((fp_r = popen("rec -t raw -b 16 -c 1 -e s -r 44100 -", "r")) == NULL)
  {
    printf("[%s] popen (rec) failed\n", mode_str);
    perror("popen");
    exit(1);
  }
  printf("[%s] Recording...\n", mode_str);

  FILE *fp_w;
  if ((fp_w = popen("play -t raw -b 16 -c 1 -e s -r 44100 -", "w")) == NULL)
  {
    printf("[%s] popen (play) failed\n", mode_str);
    perror("popen");
    exit(1);
  }
  printf("[%s] Playing...\n", mode_str);

  size_t buf_size = 1024;
  char buf[buf_size];
  while (1)
  {
    ssize_t r;

    // send
    r = read(fileno(fp_r), buf, buf_size);
    if (r == 0)
    {
      printf("[%s] read ended\n", mode_str);
      break;
    }
    if (r < 0)
    {
      printf("[%s] read failed\n", mode_str);
      perror("read");
      break;
    }
    if (send(s, buf, r, 0) < 0)
    {
      printf("[%s] send failed\n", mode_str);
      perror("send");
      break;
    }

    // receive
    r = read(s, buf, buf_size);
    if (r == 0)
    {
      printf("[%s] recv ended\n", mode_str);
      break;
    }
    if (r < 0)
    {
      printf("[%s] recv failed\n", mode_str);
      perror("recv");
      exit(1);
    }
    write(fileno(fp_w), buf, r);
  }

  shutdown(s, SHUT_WR);
  close(s);
  pclose(fp_r);
  pclose(fp_w);
  printf("[%s] Disconnected\n", mode_str);
}
