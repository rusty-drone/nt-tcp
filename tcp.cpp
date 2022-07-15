#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

int randi(int lo, int hi)
{
    int n = hi - lo + 1;
    int i = rand() % n;
    if (i < 0) i = -i;
    return lo + i;
}
int create() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        std::cerr << "Can't create a socket! Quitting" << std::endl;
        return -1;
    }
  return sock;
}
 
int main()
{
    // Create a socket
    int sock = create();

    
    std::cout << "created socket" << std::endl;
 
    // Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(8080);
    inet_pton(AF_INET, "localhost", &hint.sin_addr);
  
 
    int result = bind(sock, (sockaddr*)&hint, sizeof(hint));
 
    listen(sock, SOMAXCONN);
 
    // Wait for a connection
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
 
    int clientSocket = accept(sock, (sockaddr*)&client, &clientSize);
 
    char host[NI_MAXHOST];      // Client's remote name
    char service[NI_MAXSERV];   // Service (i.e. port) the client is connect on
 
    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);
 
    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        std::cout << host << " connected on port " << service << std::endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;
    }
 
    // Close sock socket
    close(sock);
 
    // While loop: accept and echo message back to client
    char buf[4096];
 
    while (true)
    {
      srand(time(NULL));
      int radint = randi(0, 10);
      std::cout << radint << std::endl;

      std::string s = "imu_10_10|imu_12_20|imu_13_30|";


 
        send(clientSocket, s.c_str(), s.size(), 0);
    }
 
    // Close the socket
    close(clientSocket);
 
 
        // Echo message back to client
    return 0;
}

