# Build-X-From-Scratch

This repository is a collection of projects built from scratch while learning core programming and systems concepts.

## Projects

- `HTTP server from Scratch/`
  - A basic HTTP server written in C using POSIX sockets.
  - Current state: accepts one TCP connection and responds with a minimal `HTTP/1.1 200 OK` response.

## HTTP Server (C)

Source file:

- `HTTP server from Scratch/main.c`

Build:

```bash
clang -Wall -Wextra -Wpedantic -g -o server "HTTP server from Scratch/main.c"
```

Run:

```bash
./server
```

Test from another terminal:

```bash
curl -v http://127.0.0.1:8080/
```
