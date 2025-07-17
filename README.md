# HTTP Server from Scratch

> A lightweight and minimal HTTP server written in C, based on low-level socket programming.
> Inspired by [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/).

This server is built from scratch to help understand how HTTP works at the socket level. It supports serving static files such as HTML, CSS, JavaScript, and images over HTTP/1.0 and HTTP/1.1 protocols.

---

## File Structure

```
.
├── src/                     # Core server logic
├── include/                 # Header files for modularity
├── serverroot/              # Public folder for static HTML/CSS/JS/images
├── CMakeLists.txt           # Build configuration
├── build.sh                 # Shell script to configure the build
└── README.md                # Project documentation (this file)
```

## Build & Run

### Prerequisites

* GCC or Clang
* `make`
* CMake (optional, if you're using `CMakeLists.txt`)

### Build

```bash
./build.sh     # optional: sets up the build environment
make all       # compiles the source code
```

### Run the Server

```bash
./http_server 8080
```

Visit `http://localhost:8080` to see your static files served.

## Features

* Minimal HTTP/1.0 and HTTP/1.1 server
* Serves static content: HTML, CSS, JS, images
* Clean modular C code

## Further Improvements

* Implement GET, POST, PUT, DELETE and etc requests.
* Imporve the file structure

## Testing

Drop some `.html`, `.css`, `.js`, or image files into the `serverroot/` directory.
Then run the server and open your browser at `http://localhost:<port>`.

## Credits

* Based on concepts from [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/)
