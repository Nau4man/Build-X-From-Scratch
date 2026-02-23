/*
 * Name dictionary for everything important in that code:
- AF_INET -> Address Family: Internet (IPv4) -> use IPv4 addresses.

- SOCK_STREAM -> Socket type: Stream -> TCP (reliable byte stream).

- socket() -> create a socket endpoint; returns file descriptor (fd).

- fd -> File Descriptor -> integer handle to kernel object.

- setsockopt() -> set socket option -> configure socket behavior.

- SOL_SOCKET -> Socket Option Level: socket layer.

- SO_REUSEADDR -> Socket Reuse Address -> allow quick rebinding to same port.

- sockaddr -> socket address (generic) type for APIs.

- sockaddr_in -> socket address: internet (IPv4) struct.

- sin in sockaddr_in -> historical “socket internet”.

- sin_family -> address family field.

- sin_port -> port field (16-bit).

- sin_addr.s_addr -> IPv4 address field (32-bit).

- htons -> Host To Network Short (16-bit endian conversion).

- htonl -> Host To Network Long (32-bit endian conversion).

- INADDR_ANY -> “any local interface” (0.0.0.0).

- bind() -> attach socket to local IP+port.

- listen() -> mark socket as passive/listening.

- SOMAXCONN -> max reasonable backlog from system.

- accept() -> accept one client; returns new connected socket fd.

- send() -> send bytes to connected peer.

- close() -> release fd.

- perror() -> print error using current errno.

- EXIT_FAILURE / EXIT_SUCCESS -> standard process exit codes.*
 *
 * */

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

/*
Server Side

TODO: Socket Setup (TCP Server)
TODO: Connection Handling (accept incoming connection)
TODO: Request reading (Read byte from socket into buffer)
TODO: HTTP Parsing
TODO: Routing / business logic
TODO: Response creation
TODO: Response sending
TODO: Safety & ops


Client Side

TODO: Open Socket (Open TCP connection)
TODO: HTTP request creation
TODO: Send a Request
TODO: Recieve a response
TODO: Interpretation
TODO: Browser Rendering (Presentation)


- Client asks server answers.
- TCP carries bytes; HTTPdefines what those bytes mean.


*/

#define PORT 8080  // TCP PORT number where server will listen
#define BACKLOG 10 // Max connection for listen()
#define READ_BUF_SIZE 4096

int main() {
  int server_fd;
  int client_df;
  int opt = 1; // Value passed to setsockopt to enable SO_REUSEADDR

  struct sockaddr_in
      addr; // IPv4 address structure (sin_family, sin_port, sin_addr).

  // Length type used by socket APIs (portable integer type for address lengths)

  socklen_t addr_len = sizeof(addr);

  // 1) Create TCP Soccket (IPv4 + Stream)
  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd == -1) {
    perror("Socket Failed");
    exit(EXIT_FAILURE);
  }

  // 2) Allow quick restart of server on same port (SO_REUSEADDR)
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) ==
      -1) {

    perror("setsocket failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // 3) Fill serve address struct

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(8080);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // 4) Bind Socket to IP:PORT

  if (bind(server_fd, (struct sockaddr *)&addr, addr_len) == -1) {
    perror("Bind Failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // 5) Mark as listening socket
  if (listen(server_fd, SOMAXCONN) == -1) {
    perror("Listen Failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port 8080...\n");

  // 6) Acccept one incoming connection (Blocking all)

  client_df = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
  if (client_df == -1) {
    perror("accept failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }
  /*
    // 7) Send data to connected client
    const char *msg = "Hello World\n";
    if (send(client_df, msg, 12, 0) == -1) {
      perror("send failed");
      close(client_df);
      close(server_fd);
      exit(EXIT_FAILURE);
    }
  */

  const char *body = "Hello, HTTP!\n";
  const char *response = "HTTP/1.1 200 OK\r\n"
                         "Content-Type: text/plain\r\n"
                         "Content-Length: 13\r\n"
                         "Connection: close\r\n"
                         "\r\n"
                         "Hello, HTTP!\n";
  if (send(client_df, response, strlen(response), 0) == -1) {
    perror("send failed");
    close(client_df);
    close(server_fd);
    exit(EXIT_FAILURE);
  }
  (void)body;
  // 8) Close client connection, then server socket
  close(client_df);
  close(server_fd);

  return EXIT_SUCCESS;
}
