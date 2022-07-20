#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <netdb.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <type_traits>
#include <unistd.h>

#include <chrono>
#include <vector>

#include <wiringSerial.h>

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

  // Bind the ip address and port to a socket
  sockaddr_in hint;
  hint.sin_family = AF_INET;
  hint.sin_port = htons(8080);
  inet_pton(AF_INET, "localhost", &hint.sin_addr);

  int result = bind(sock, (sockaddr *)&hint, sizeof(hint));

  listen(sock, SOMAXCONN);

  std::cout << "created socket" << std::endl;
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

  int serialFD;
  if ((serialFD = serialOpen("/dev/ttyACM0", 500000)) < 0) {
    std::cerr << "Unable to Open Serial Device" << std::endl;
  }

  while (true) {

    int charNumber;
    std::string serialName;
    float serialValue;
    std::string serialData = "";
    if (serialGetchar(serialFD) == 60) {
      while ((charNumber = serialGetchar(serialFD)) != 62) {
        char charData = charNumber;
        serialData.push_back(charData);
      }
      // if ((serialData.rfind("!", 0) == 0) && (serialData.rfind("?") ==
      // serialData.size() - 1)) {
      // std::cout << "serial Data: " << serialData << std::endl;
      int delim = serialData.find(":");
      serialName = serialData.substr(0, delim);
      serialValue =
          std::stof(serialData.substr(delim + 1, serialData.size() - 1));

      std::cout << serialName << "\t\t\t" << serialValue << std::endl;
      // }
    }
    for (int i = 0; graphers.size(); i++) {
      if (graphers[i].name == serialName) {

        data2 = serialValue;
      }
    }

    for (int i = 0; i < graphers.size(); i++) {
      send(clientSocket, graphers[i].get_parsed_data().c_str(),
           graphers[i].get_parsed_data().size(), 0);
    }

    data += 1;
    data3 -= 1;
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
