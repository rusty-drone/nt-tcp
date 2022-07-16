#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <netdb.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <chrono>
#include <vector>
using namespace std::chrono;

#include "grapher.h"

int randi(int lo, int hi) {
  int n = hi - lo + 1;
  int i = rand() % n;
  if (i < 0)
    i = -i;
  return lo + i;
}
int create() {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    std::cerr << "Can't create a socket! Quitting" << std::endl;
    return -1;
  }
  return sock;
}

int main() {

  std::vector<Grapher> graphers;
  // Create a socket
  int sock = create();

  std::string s = "imu_10_10|gyro_20_20|dick-size_10_300";
  std::cout << "created socket" << std::endl;
  // Bind the ip address and port to a socket
  sockaddr_in hint;
  hint.sin_family = AF_INET;
  hint.sin_port = htons(8080);
  inet_pton(AF_INET, "localhost", &hint.sin_addr);

  int result = bind(sock, (sockaddr *)&hint, sizeof(hint));

  listen(sock, SOMAXCONN);

  // Wait for a connection
  sockaddr_in client;
  socklen_t clientSize = sizeof(client);

  int clientSocket = accept(sock, (sockaddr *)&client, &clientSize);

  char host[NI_MAXHOST];    // Client's remote name
  char service[NI_MAXSERV]; // Service (i.e. port) the client is connect on

  memset(host, 0, NI_MAXHOST);
  memset(service, 0, NI_MAXSERV);

  if (getnameinfo((sockaddr *)&client, sizeof(client), host, NI_MAXHOST,
                  service, NI_MAXSERV, 0) == 0) {
    std::cout << host << " connected on port " << service << std::endl;
  } else {
    inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
    std::cout << host << " connected on port " << ntohs(client.sin_port)
              << std::endl;
  }

  // Close sock socket
  close(sock);

  // While loop: accept and echo message back to client
  char buf[4096];

  std::chrono::milliseconds t =
      duration_cast<milliseconds>(system_clock::now().time_since_epoch());

  long long micros =
      std::chrono::duration_cast<std::chrono::microseconds>(t).count();

  float data = 0.0;
  float data2 = 0.0;
  float data3 = 0.0;
  float data4 = 0.0;
  float data5 = 0.0;
  float data6 = 0.0;
  float data7 = 0.0;
  float data8 = 0.0;
  float data9 = 0.0;

  Grapher g("imu", &data, micros);
  Grapher g2("gyro", &data2, micros);
  Grapher g3("a", &data3, micros);
  Grapher g4("b", &data4, micros);
  Grapher g5("c", &data5, micros);
  Grapher g6("d", &data6, micros);
  Grapher g7("e", &data7, micros);
  Grapher g8("f", &data8, micros);
  Grapher g9("g", &data9, micros);
  std::cout << g.get_parsed_data() << std::endl;

  graphers.push_back(g);
  graphers.push_back(g2);
  graphers.push_back(g3);
  graphers.push_back(g4);
  graphers.push_back(g5);
  graphers.push_back(g6);
  graphers.push_back(g7);
  graphers.push_back(g8);
  graphers.push_back(g9);

  while (true) {
    for (int i = 0; i < graphers.size(); i++) {
      send(clientSocket, graphers[i].get_parsed_data().c_str(),
           graphers[i].get_parsed_data().size(), 0);
    }
    data += 1;
    data2 -= 1;
    data3 = randi(0, 100);
    data4 = randi(0, 100);
    data5 = randi(0, 100);
    data6 = randi(0, 100);
    data7 = randi(0, 100);
    data8 = randi(0, 100);
    data9 = randi(0, 100);
    usleep(100 * 1000);
  }

  // Close the socket
  close(clientSocket);

  // Echo message back to client
  return 0;
}
