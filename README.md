
# HTTP Server in C++98

## Description

This project involves implementing a simple HTTP server in C++98, meeting several functional and performance requirements. The server must support HTTP 1.1, including GET, POST, and DELETE methods, while being non-blocking, highly performant, and configurable through a configuration file.

The server should be able to serve static websites, handle file uploads, and execute CGI scripts, all while adhering strictly to the requirements of non-blocking I/O, multi-port listening, and efficient resource usage.

### Functional Requirements

#### Non-blocking I/O:
    The server use a single select() for managing all input/output operations.
    The server never block, and its handle multiple clients concurrently.
    select() check for both read and write operations simultaneously.

#### Configuration File:
    The server should read from a configuration file to set parameters such as port, host, routes, default error pages, client body size limits, etc.
    The configuration file should allow flexibility to set server names, redirection rules, static file locations, and CGI handling.
    If no configuration file is provided, the server must use default values.

#### Supported HTTP Methods:
    GET, POST, and DELETE methods must be implemented.
    Support for file uploads via POST.
    Static file serving and directory listing are available.

#### CGI Support:

    The server support executing CGI scripts for certain file extensions (eg  .py).
    The server  properly handle chunked transfer encoding and terminate CGI outputs when needed.
    CGI scripts run in the correct directory for relative file access.
    CGI invoked with the requested file path as the first argument.

#### Concurrency:

    The server remain responsive even under stress and high traffic.
    Multiple ports supported (configured in the configuration file).
    The server serve content without hanging or crashing.

#### Error Handling:

    The server send correct HTTP response status codes.

#### File System & Routing:

    URL routes mapped to specific directories or files on the file system.
    The server maps URLs to their respective files, supporting nested directories.
    directory listings supported.
