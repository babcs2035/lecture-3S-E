#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define BUF_SIZE 512
#define DEBUG

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
    printf("[client] Trying to connect to %s:%d...\n", inet_ntoa(server_addr.sin_addr), port);
    if (connect(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
    {
      printf("[client] Connect to %s:%d failed\n", inet_ntoa(server_addr.sin_addr), port);
      perror("[client] connect");
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

  char buf[BUF_SIZE];
  while (1)
  {
    // send
    ssize_t r = read(fileno(fp_r), buf, BUF_SIZE);
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
    r = read(s, buf, BUF_SIZE);
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
