#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <vector>
#include <thread>
#include <mutex>
#define BUF_SIZE 512
// #define DEBUG

struct client
{
  int socket;
  struct sockaddr_in client_addr;
  int data_size;
  char *data;
  bool has_dead = false;
};

int listen_socket;
std::vector<client> clients;
std::mutex mtx;
bool has_dead_client = false;
char merged_voice[BUF_SIZE];

void init_listen(int port)
{
  listen_socket = socket(PF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;
  if (bind(listen_socket, (struct sockaddr *)&addr, sizeof(addr)) != 0)
  {
    perror("[server] bind failed");
    exit(1);
  }
  listen(listen_socket, 16);
  printf("[server] Listening on port %d...\n", port);
}

void add_client()
{
  printf("[server] Adding clients...\n");
  while (true)
  {
    client client;
    socklen_t client_addr_len = sizeof(client.client_addr);
    client.socket = accept(listen_socket, (struct sockaddr *)&client.client_addr, &client_addr_len);
    if (client.socket < 0)
    {
      printf("[server] Accept at %s:%d failed\n", inet_ntoa(client.client_addr.sin_addr), ntohs(client.client_addr.sin_port));
      perror("[server] accept");
      return;
    }
    client.data = (char *)malloc(BUF_SIZE * sizeof(char));
    std::lock_guard<std::mutex> lock(mtx);
    clients.push_back(client);
    printf("[server] Connected to %s:%d\n", inet_ntoa(client.client_addr.sin_addr), ntohs(client.client_addr.sin_port));
  }
}

void remove_client()
{
  std::vector<client> new_clients;
  for (auto client : clients)
  {
    if (client.has_dead)
    {
      printf("[server] Disconnected from %s:%d\n", inet_ntoa(client.client_addr.sin_addr), ntohs(client.client_addr.sin_port));
      close(client.socket);
      free(client.data);
    }
    else
    {
      new_clients.push_back(client);
    }
  }
  std::lock_guard<std::mutex> lock(mtx);
  clients.clear();
  clients = new_clients;
}

void receive_voice(size_t index)
{
  std::lock_guard<std::mutex> lock(mtx);
  auto &client = clients[index];
  if (client.has_dead)
  {
    printf("[server] skip receive from client %ld (%s:%d)\n", index, inet_ntoa(client.client_addr.sin_addr), ntohs(client.client_addr.sin_port));
    return;
  }

  client.data_size = read(client.socket, client.data, BUF_SIZE);
  if (client.data_size == 0)
  {
    printf("[server] receive from %s:%d ended\n", inet_ntoa(client.client_addr.sin_addr), ntohs(client.client_addr.sin_port));
    client.has_dead = true;
    has_dead_client = true;
    return;
  }
  if (client.data_size < 0)
  {
    printf("[server] receive from %s:%d failed\n", inet_ntoa(client.client_addr.sin_addr), ntohs(client.client_addr.sin_port));
    perror("[server] read");
    client.has_dead = true;
    has_dead_client = true;
    return;
  }
#ifdef DEBUG
  printf("[server] received %d bytes from client %ld (%s:%d)\n", client.data_size, index, inet_ntoa(client.client_addr.sin_addr), ntohs(client.client_addr.sin_port));
#endif
}

void receive_voices()
{
  for (size_t i = 0; i < clients.size(); ++i)
  {
    std::thread(receive_voice, i).join();
  }
}

void send_voice(size_t index)
{
  std::lock_guard<std::mutex> lock(mtx);
  auto &client = clients[index];
  if (client.has_dead)
  {
    printf("[server] skip send to client %ld (%s:%d)\n", index, inet_ntoa(client.client_addr.sin_addr), ntohs(client.client_addr.sin_port));
    return;
  }
  int s = send(client.socket, merged_voice, BUF_SIZE, 0);
  if (s < 0)
  {
    printf("[server] send to %s:%d failed\n", inet_ntoa(client.client_addr.sin_addr), ntohs(client.client_addr.sin_port));
    perror("[server] send");
    client.has_dead = true;
    has_dead_client = true;
    return;
  }
#ifdef DEBUG
  printf("[server] sent %d bytes to client %ld (%s:%d)\n", s, index, inet_ntoa(client.client_addr.sin_addr), ntohs(client.client_addr.sin_port));
#endif
}

void send_voices()
{
  memset(merged_voice, 0, BUF_SIZE);
  for (auto client : clients)
  {
    if (client.has_dead)
    {
      printf("[server] skip merge from %s:%d\n", inet_ntoa(client.client_addr.sin_addr), ntohs(client.client_addr.sin_port));
      continue;
    }
    for (int i = 0; i < client.data_size; ++i)
    {
      merged_voice[i] += client.data[i];
    }
  }

  for (size_t i = 0; i < clients.size(); ++i)
  {
    std::thread(send_voice, i).join();
  }
}

void manage_voice()
{
  printf("[server] Managing voices...\n");
  while (true)
  {
    receive_voices();
    send_voices();
    if (has_dead_client)
    {
      remove_client();
      has_dead_client = false;
    }
  }
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("[server] Usage: %s <port>\n", argv[0]);
    exit(1);
  }
  const int port = atoi(argv[1]);
  init_listen(port);

  std::thread add_th(add_client);
  std::thread manage_th(manage_voice);
  add_th.join();
  manage_th.join();

  shutdown(listen_socket, SHUT_WR);
  close(listen_socket);
  printf("[server] Terminated\n");
}
