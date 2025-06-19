# HTTP Server from Scratch

> [Resource](https://beej.us/guide/bgnet/html/) 

A lightweight and minimal HTTP server written in C. This server is built from scratch using low-level socket
programming, capable of serving static files (HTML, CSS, JS, images) over HTTP 1.0/1.1.

## File Structure

```bash
.
├── src/                     # Core server code
│   ├── main.c               # Entry point: socket setup and accept loop
│   ├── server.c             # Request dispatching, connection worker
│   ├── http.c               # HTTP request parser & response builder
│   ├── file.c               # Static file loader (e.g. file_load())
│   ├── mime.c               # MIME type detection based on file extensions
│   └── cache.c              # LRU cache to speed up static files
│
├── include/                  # Header files
│   ├── http.h
│   ├── file.h
│   ├── mime.h
│   └── cache.h
│
├── serverroot/               # static HTML, CSS, JS, images here
├── CMakeLists.txt
└── README.md

```