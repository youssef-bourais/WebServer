

#include "RequestParsser.hpp"
#include <cstdlib>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include "../Tools/Tools.hpp"


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

RequestParsser::RequestParsser(int fd) : HttpVersion("HTTP/1.1"), Body(""), remain("") , Fd(fd), flage(false)
{
    std::string Line;
    char Buffer[2024 + 1];
    int r = 0;
    int Tot = 0;
    int HeadersEnd = 0;
    std::string tmp;
    
    while (true) 
    {
        r = recv(fd, Buffer, 20, 0);
        if(r <= 0)
            break;
        Buffer[r] = '\0';
        Tot += r;
        tmp.append(Buffer, r);
        HeadersEnd = tmp.find("\r\n\r\n");
        if((unsigned long)HeadersEnd != std::string::npos)
            break;
    }

    this->Line = tmp.substr(0, HeadersEnd);
    this->HttpHeaders = InitHttpheaders(tmp);
    this->HttpVersion = GetVersion(this->Line);
    this->Method = GetMethod(this->Line);
    std::string string;
    this->Path = getPath(this->Line, string);
    
    if(string[string.size() - 1] == '/')
    {
        this->flage = true;
        std::cout << "parss: "<< this->flage<<std::endl;
    }
    if (!this->GetHeader("Transfer-Encoding").empty() && this->GetHeader("Transfer-Encoding") == "chunked") 
    {
        if(this->Line.size() < tmp.size())
        {
            std::string remain = tmp.substr(HeadersEnd + 8, tmp.size());
            this->remain = remain;
        }
        ReadChunkedBody(fd);
        // std::cout << this->Body<<std::endl;
    }
    else if (!this->GetHeader("Content-Length").empty()) 
    {

        std::string remain = tmp.substr(HeadersEnd + 4, tmp.size());
        this->remain = remain;
        ReadBody(fd, this->GetHeader("Content-Length"));
    }
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

