#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
  int s;
  if (argc == 3)
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
      printf("[client] Connect to %s:%d failed", inet_ntoa(server_addr.sin_addr), port);
      perror("connect");
      exit(1);
    }
    printf("[client] Connected to %s:%d\n", inet_ntoa(server_addr.sin_addr), port);
  }
  else
  {
    printf("[client] Usage: %s <address> <port>\n", argv[0]);
    exit(1);
  }

  FILE *fp_r;
  if ((fp_r = popen("rec -t raw -b 16 -c 1 -e s -r 44100 -", "r")) == NULL)
  {
    perror("[client] popen (rec) failed");
    exit(1);
  }
  printf("[client] Recording...\n");

  FILE *fp_w;
  if ((fp_w = popen("play -t raw -b 16 -c 1 -e s -r 44100 -", "w")) == NULL)
  {
    perror("[client] popen (play) failed");
    exit(1);
  }
  printf("[client] Playing...\n");

  size_t buf_size = 1 << 12;
  char buf[buf_size];
  while (1)
  {
    ssize_t r;

    // send
    r = read(fileno(fp_r), buf, buf_size);
    if (r == 0)
    {
      printf("\n[client] read ended\n");
      break;
    }
    if (r < 0)
    {
      perror("\n[client] read failed");
      break;
    }
    if (send(s, buf, r, 0) < 0)
    {
      perror("\n[client] send failed");
      break;
    }

    // receive
    r = read(s, buf, buf_size);
    if (r == 0)
    {
      printf("\n[client] recv ended\n");
      break;
    }
    if (r < 0)
    {
      perror("\n[client] recv failed");
      exit(1);
    }
    write(fileno(fp_w), buf, r);
  }

  shutdown(s, SHUT_WR);
  close(s);
  pclose(fp_r);
  pclose(fp_w);
  printf("\n[client] Disconnected\n");
}
