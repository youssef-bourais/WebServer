

#include "RequestParsser.hpp"
#include <cstdlib>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include "../Tools/Tools.hpp"
#include <fstream>
#include <sstream>


std::string trim(const std::string &str)
{
    std::string result = str;

    if (result.length() >= 2 && result.substr(0, 2) == "\r\n")
    {
        result = result.substr(2);
    }
    if (result.length() >= 2 && result.substr(0, 2) == "\r\n")
    {
        result = result.substr(2);
    }

    if (result.length() >= 2 && result.substr(result.length() - 2) == "\r\n")
    {
        result = result.substr(0, result.length() - 2);
    }

    return result;
}

std::string Start(const std::string &request)
{
    std::string boundary = "";
    int i = request.find("boundary=") + 9;
    while (request[i] != '\r' && request[i] != '\n' && request[i] != '\0')
    {
        boundary += request[i];
        i++;
    }
    return boundary;
}

void sub_parse(std::istringstream &stream, std::string start, std::string end, RequestBody &req)
{
    std::string line;
    std::string tmp = "";
    while (line.find(start) == std::string::npos)
        std::getline(stream, line);
    while (std::getline(stream, line))
    {
        if (line.find("Content-Disposition: form-data") != std::string::npos)
        {
            int i = line.find("name=") + 6;
            req.name = "";
            while (line[i] != '"' && line[i] != '\r' && line[i] != '\r' && line[i] != '\0')
                req.name += line[i++];
            i = line.find("filename=") + 10;
            req.filename = "";
            while (line[i] != '"' && line[i] != '\r' && line[i] != '\r' && line[i] != '\0')
                req.filename += line[i++];
        }
        else if (line.find("Content-Type:") != std::string::npos)
            continue;
        else if (line.find(end) != std::string::npos)
        {
            req.data += tmp;
            break;
        }
        else if (line.find("--" + start) != std::string::npos)
            continue;
        else
            req.data += line + "\n";
    }
    req.data = trim(req.data);
}

void ft_parse(std::string str, RequestBody &req)
{
    std::istringstream stream(str);
    std::string start = Start(str);
    std::string end = start + "--";
    sub_parse(stream, start, end, req);
}

std::string handleBoundry(int fd)
{
    std::string line;
    int buffer_s = 100;
    char ch[100];
    ssize_t bytesRead;

    while (true)
    {
        bytesRead = read(fd, ch, buffer_s);

        if (bytesRead <= 0)
            break;
        line.append(ch, bytesRead);
    }
    //std::cout << "[handleBoundry - DATA ] => " + line + "\n";
    // std::istringstream iss(line);
    // std::string start = Start(line);
    // Request req = sub_parse(iss,);
    // std::cout <<"req.data => "  +  req.data << std::endl;
    return line;
}

RequestParsser &RequestParsser::operator=(const RequestParsser &src) 
{
    if(this != &src)
    {
        HttpHeaders.clear();
        std::list<KeyValuee>::const_iterator it = src.HttpHeaders.begin();
        while(it != src.HttpHeaders.end())
        {
            HttpHeaders.push_back(*it); 
            it++;
        }
        this->HttpVersion = src.HttpVersion;
        this->Body = src.Body;
        this->Method = src.Method;
        this->Path = src.Path;
        this->Line = src.Line;
        this->remain = src.remain;
        this->flage = src.flage;
        this->name = src.name;
        this->filename = src.filename;
        this->data = src.data;
    }
    return *this;
}

RequestParsser::RequestParsser(const RequestParsser& copy)
{
    *this = copy;
}

RequestParsser::~RequestParsser()
{
}

std::string GetVersion(std::string RecivedLine)
{
    int first_space = RecivedLine.find(' ');
    int start = RecivedLine.find(' ', first_space + 1);
    int end = RecivedLine.find('\n');
    std::string n = RecivedLine.substr(start + 1, end - start - 2);
    return n;
}

std::string GetMethod(std::string RecivedLine)
{
    int pos = RecivedLine.find(' ');
    std::string Method = RecivedLine.substr(0, pos);
    return(Method);
}


void NormalizePath(std::string &Path)
{

    std::string Cor;
    int i = 0;
    while (i < (int)Path.length()) 
    {
        Cor += Path[i];
        if (Path[i] == '/') 
        {
            while (Path[i] == '/') 
                i++;
            continue;
        }
        i++;
    }
    Path = Cor;
}


//checking for / at the begining and at the end and remove consecutive slashes
void ParssPath(std::string &path)
{    
    if(path.length() == 1)
        return;
    else 
    {
        std::string tmp ;
        if (!path.empty())
            tmp = path.substr(1); // remove the first slash /
        int value = checkFileType(tmp);
        if(value == FILE_TYPE)
        {
            path = tmp;
            return ;
        }
        else if(value == DIR_TYPE)
        {
            if (tmp[tmp.size() - 1] != '/')
                tmp += '/';
            path = tmp;
            return ;
        }
        else 
        {
            if (!tmp.empty() && tmp[tmp.size() - 1]  == '/') 
                tmp.erase(tmp.size() - 1);
            // value = checkFileType(tmp);
            // if(value == FILE_TYPE)
            path = tmp;
            // else 
            //     path = "";
        }
    }
}

std::string getPath(std::string RecivedLine, std::string &oldPath)
{
    int first_space = RecivedLine.find(' ');
    int second_space = RecivedLine.find(' ', first_space + 1);
    std::string path = RecivedLine.substr(first_space + 1, second_space - first_space - 1);
    // if(path.find("?") != std::string::npos)
    // {
    //     oldPath = path;
    //     // path = path.substr(0, path.find("?"));
    // }
    oldPath = path;
    NormalizePath(path);
    ParssPath(path);
    return path;
}

std::list<KeyValuee> InitHttpheaders(std::string Line)
{
    std::list<KeyValuee> headers;
    int start_pos = Line.find("\r\n") + 2;
    std::string tmp;

    while (start_pos < (int)Line.size()) 
    {
        if(Line[start_pos] == '\r')
        {
            start_pos += 2;
            unsigned long del = tmp.find(":");
            if(del == std::string::npos)
                continue;
            std::string key = tmp.substr(0, del);
            std::string value = tmp.substr(del + 2, tmp.length());
            headers.push_back(KeyValuee(key,value));
            tmp = "";
            continue;
        }
        tmp += Line[start_pos];
        start_pos++;
    }
    return headers;
}

void RequestParsser::PrintHeaders() const
{
    std::list<KeyValuee>::const_iterator it = HttpHeaders.begin();
    while (it != HttpHeaders.end()) 
    {
        std::cout << it->HttpHeader<< ":"<< it->HttpValue<<std::endl;
        it++;
    }
}

std::string RequestParsser::GetHeader(std::string key) const
{
    std::string Value;
    std::list<KeyValuee>::const_iterator it = this->HttpHeaders.begin();
    while(it != this->HttpHeaders.end())
    {
        if(key == it->HttpHeader)
        {
            Value = it->HttpValue;
            return Value;
        }
        it++;
    }
    Value = "";
    return Value;
}

void RequestParsser::ReadBody(int fd, const std::string &lengthStr) 
{
    int length = StringToInt(lengthStr);
    char buffer[1024];
    this->Body += this->remain;
    
    int bytesRead = 0;
    while (bytesRead < length) 
    {
        int r = recv(fd, buffer, 1024, 0);
        if (r <= 0)
            break;
        buffer[r] = '\0';
        this->Body.append(buffer, r);
        bytesRead += r;
    }
}

void RequestParsser::ReadChunkedBody(int fd) 
{
    const int bufferSize = 1;
    char buffer[bufferSize];
    std::string request;
    bool chunkedComplete = false;

    
    request +=this->remain;
    
    while (!chunkedComplete) 
    {
        int bytesRead = recv(fd, buffer, bufferSize, 0);
        if (bytesRead <= 0) 
        {
            break;
        }

        request.append(buffer, bytesRead);

        size_t endChunked = request.find("0\r\n\r\n");
        if (endChunked != std::string::npos) 
        {
            chunkedComplete = true;
        }
    }
    
    char ch;
    while (true) 
    {
        std::string lengthStr;
        int r = recv(fd, &ch, 1, 0);
        if (r <= 0) 
            break;
        while (ch != '\r') 
        {
            lengthStr += ch;
            r = recv(fd, &ch, 1, 0);
            if (r <= 0) 
                break;
        }
        recv(fd, &ch, 1, 0);  // Skip \n

        int length = std::strtol(lengthStr.c_str(), NULL, 16);
        if (length == 0) 
            break;  // Last chunk

        // Read the chunk data
        int bytesRead = 0;
        char buffer[1024];
        while (bytesRead < length) 
        {
            r = recv(fd, buffer, std::min(length - bytesRead, 1024), 0);
            if (r <= 0) 
            {
                std::cerr << "Failed to read chunk data or connection closed" << std::endl;
                break;
            }
            Body.append(buffer, r);
            bytesRead += r;
        }
        if (bytesRead < length) 
            break;

        // Skip \r\n after chunk data
        recv(fd, &ch, 1, 0);
        if (ch != '\r') 
            break;
        recv(fd, &ch, 1, 0);
        if (ch != '\n') 
            break;
    }
}

bool RequestParsser::GetFlage() const
{
    return this->flage;
}

// std::string handleChunkedTransfer(int socket) 
// {
//     std::string body;
//     while (true) 
//     {
//         std::string chunkSizeHex = readLine(socket);
//
//         if (chunkSizeHex.empty()) 
//         {
//             break; // Handle error: empty line received
//         }
//
//         int chunkSize = std::strtol(chunkSizeHex.c_str(), NULL, 16);
//
//         if (chunkSize == 0) 
//         {
//             readLine(socket); // Read and discard trailer (optional)
//             break;
//         }
//
//         std::vector<char> chunkData(chunkSize);
//         ssize_t totalBytesReceived = 0;
//         while (totalBytesReceived < chunkSize) 
//         {
//             ssize_t bytesRead = recv(socket, chunkData.data() + totalBytesReceived, chunkSize - totalBytesReceived, 0);
//             if (bytesRead <= 0) 
//             {
//                 break; // Handle error: socket error or connection closed
//             }
//             totalBytesReceived += bytesRead;
//         }
//         if (totalBytesReceived != chunkSize) 
//         {
//             break;
//         }
//
//         body += std::string(chunkData.data(), chunkSize);
//         readLine(socket); // Read and discard CRLF after chunk data (optional)
//   }
//   return body;
// }

std::string readLine(int socket) 
{
    std::string line;
    char c;
    while (true) 
    {
        ssize_t bytesRead = recv(socket, &c, 1, 0);
        if (bytesRead <= 0) 
            break;
        if (c == '\r') 
        {
            bytesRead = recv(socket, &c, 1, 0);
            break;
        }
        line += c;
    }
    return line;
}

std::string handleChunkedTransfer(int socket) 
{
    std::string body;
    while (true) 
    {
        std::string chunkSizeHex = readLine(socket);

        int chunkSize = std::strtol(chunkSizeHex.c_str(), NULL, 16);
        if (chunkSize == 0) 
        {
            break;
        }
        std::vector<char> chunkData(chunkSize);
        int i = 0;
        while(i < chunkSize)
        {
            ssize_t bytesRead = recv(socket, chunkData.data() + i, chunkSize - i, 0);

        
            // body.append(chunkData.data(), bytesRead);
            if (bytesRead <= 0) 
                break;
            i += bytesRead;
        }
        if(i != chunkSize)
        {
            break;
        }
        body.append(chunkData.data(), chunkSize);
        readLine(socket);
    }
    return body;
}
std::string RequestParsser::Getname() const
{
    return this->name;
}

std::string RequestParsser::Getfilename() const
{
    return this->filename;
}

std::string RequestParsser::GetData() const
{
    return this->data;
}

RequestParsser::RequestParsser(int fd) : HttpVersion("HTTP/1.1"), Body(""), filename("") , Fd(fd), flage(false)
{
    this->Line = readLine(fd);
   
    this->HttpVersion = GetVersion(this->Line);
    this->Method = GetMethod(this->Line);
    
    std::string forDelete;
    
    this->Path = getPath(this->Line, forDelete);
    this->remain = forDelete;
    if(forDelete[forDelete.size() - 1] == '/')
    {
        this->flage = true;
    }

    std::string headers;
    std::string contentType;
    std::string transferEncoding;
    std::string contentLength;
    std::string boundary;
    
    while (true) 
    {
        std::string headerLine = readLine(fd);

        headers += headerLine + "\r\n";
        if (headerLine.empty()) 
            break;

        if (headerLine.find("Content-Type:") != std::string::npos) 
        {
            contentType = headerLine.substr(headerLine.find(":") + 2);
            size_t pos = contentType.find("boundary=");
            if (pos != std::string::npos) 
            {
                boundary = contentType.substr(pos + 9, contentType.size() - pos);
            }
        }
        if (headerLine.find("Transfer-Encoding:") != std::string::npos) 
        {
            transferEncoding = headerLine.substr(headerLine.find(":") + 2);
        }
        if (headerLine.find("Content-Length:") != std::string::npos) 
        {
            contentLength = headerLine.substr(headerLine.find(":") + 2);
        }
    }
    // std::cout << headers<<std::endl;
    // exit(0);

    if(!transferEncoding.empty())
    {
        std::string key = "Transfer-Encoding";
        std::string value = transferEncoding;
        this->HttpHeaders.push_back(KeyValuee(key, value));

    }
    if(!contentLength.empty())
    {

        std::string key = "Content-Length";
        std::string value = contentLength;
        this->HttpHeaders.push_back(KeyValuee(key, value));

    }
    if(!contentType.empty())
    {
        std::string key = "Content-Type";
        std::string value = contentType.substr(0, contentType.find(";"));
        this->HttpHeaders.push_back(KeyValuee(key, value));
    }
    if (!boundary.empty()) 
    {
        RequestBody req;
        this->Body = handleBoundry(fd);
        ft_parse(this->Body, req);
        this->filename = req.filename;
        this->data = req.data;
        this->name = req.name;
    } 
    else if (!transferEncoding.empty() && transferEncoding.find("chunked") != std::string::npos) 
    {
        this->Body = handleChunkedTransfer(fd);
    }
    else if (!contentLength.empty()) 
    {
        ReadBody(fd, this->GetHeader("Content-Length"));
        // std::cout << this->Body<<std::endl;
        // exit(0);
    }
    // std::cout << this->Body.size()<<std::endl;
    // std::cout << this->Body<<std::endl;
    // exit(0);
    
}

std::string RequestParsser::GetPath() const
{
    return this->Path;
}

std::string RequestParsser::GetBody() const
{
    return this->Body;
}

RequestParsser::RequestParsser()
{

}

std::string RequestParsser::GetHttpMethod() const
{
    return this->Method;
}

