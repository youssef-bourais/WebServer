

#include "RequestParsser.hpp"
#include <sys/socket.h>
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
    NormalizePath(path);
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
            value = checkFileType(tmp);
            if(value == FILE_TYPE)
                path = tmp;
            // else 
            //     path = "";
        }
    }
}

std::string getPath(std::string RecivedLine)
{
    int first_space = RecivedLine.find(' ');
    int second_space = RecivedLine.find(' ', first_space + 1);
    std::string path = RecivedLine.substr(first_space + 1, second_space - first_space - 1);
    ParssPath(path);
    return path;
}

std::list<KeyValuee> InitHttpheaders(std::string Line)
{
    std::list<KeyValuee> headers;
    int start_pos = Line.find("\r\n") + 2;
    std::string tmp;

    while (start_pos < (int)Line.length()) 
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

RequestParsser::RequestParsser(int fd) : Fd(fd), HttpVersion("HTTP/1.1"), Body("")
{
    std::string Line;
    char Buffer[1024];
    int r = 0;
    int Tot = 0;
    int HeadersEnd = 0;
    std::string tmp;
    
    while (true) 
    {
        r = recv(fd, Buffer, 1024, 0);
        if(r <= 0)
            break;
        Tot += r;
        tmp.append(Buffer, r);
        HeadersEnd = tmp.find("\r\n\r\n");
        if(HeadersEnd != std::string::npos)
            break;
    }

    this->Line = tmp.substr(0, HeadersEnd);
    this->HttpVersion = GetVersion(this->Line);
    this->Method = GetMethod(this->Line);
    this->HttpHeaders = InitHttpheaders(this->Line);



    //
    // std::string remainBody = tmp.substr(HeadersEnd + 4, tmp.size() - HeadersEnd);
    // if()
    // {
    //
    // }
    // std::cout << Body<<std::endl;

}


