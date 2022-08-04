#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#include <chrono>
#include <vector>

#include <wiringSerial.h>

#include "grapher.h"

using namespace std::chrono;

const int PORT = 8080;
const char IP_ADDRESS[] = "localhost";

struct ParsedSerialData {
    std::string name;
    float value{};
};

int create() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Can't create a socket! Quitting" << std::endl;
        return -1;
    }
    return sock;
}

ParsedSerialData
parse_serial_data(int serialFD) {
    int charNumber;
    std::string serialData;
    if (serialGetchar(serialFD) == 60) {
        while ((charNumber = serialGetchar(serialFD)) != 62) {
            char charData = charNumber;
            serialData.push_back(charData);
        }
        int delim = serialData.find(':');
        ParsedSerialData data;
        data.name = serialData.substr(0, delim);
        data.value = std::stof(serialData.substr(delim + 1, serialData.size() - 1));
        return data;
    }
}

int main() {
    std::vector<Grapher> graphers;

    int sock = create();

    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port = htons(PORT);
    inet_pton(AF_INET, IP_ADDRESS, &hint.sin_addr);

    if (bind(sock, (sockaddr *) &hint, sizeof(hint)) != 0) {
        std::cerr << "Can't bind socket to port: " << hint.sin_port << std::endl;
        return 1;
    }

    if (listen(sock, SOMAXCONN) == -1) {
        std::cerr << "Can't listen in socket" << std::endl;
        return 1;
    }

    listen(sock, SOMAXCONN);
    std::cout << "listening on " << IP_ADDRESS << ": " << PORT << std::endl;
    sockaddr_in client{};
    socklen_t clientSize = sizeof(client);

    int clientSocket = accept(sock, (sockaddr *) &client, &clientSize);

    char host[NI_MAXHOST];    // Client's remote name
    char service[NI_MAXSERV]; // Service (i.e. port) the client is connected on

    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);

    if (getnameinfo((sockaddr *) &client, sizeof(client), host, NI_MAXHOST,
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

    Grapher g("imu", &data, micros);
    Grapher g2("gyro", &data2, micros);
    Grapher g3("a", &data3, micros);
    Grapher g4("accelerometer", &data4, micros);

    std::cout << g.get_parsed_data() << std::endl;

    graphers.push_back(g);
    graphers.push_back(g2);
    graphers.push_back(g3);
    graphers.push_back(g4);

    int serialFD;
    if ((serialFD = serialOpen("/dev/ttyACM0", 115200)) < 0) {
        std::cerr << "Unable to Open Serial Device" << std::endl;
    }

    while (true) {
        ParsedSerialData serialData = parse_serial_data(serialFD);

        for (auto &grapher: graphers) {
            if (grapher.name == serialData.name) {
                *grapher.value = serialData.value;
            }
        }

        for (auto &grapher: graphers) {
            send(clientSocket, grapher.get_parsed_data().c_str(),
                 grapher.get_parsed_data().size(), 0);
        }

        data += 1;
        data3 -= 1;
        usleep(100 * 1000);
    }
}
