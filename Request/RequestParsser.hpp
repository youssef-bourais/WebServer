
#pragma once


#include <iostream>

#include <list>
#include <string>

struct KeyValuee 
{
    std::string HttpHeader;
    std::string HttpValue;
    KeyValuee(const std::string &k, const std::string &v) : HttpHeader(k), HttpValue(v) {}
};

struct RequestBody
{
    std::string filename;
    std::string name;
    std::string data;

};

class RequestParsser
{
    protected:
        std::string HttpVersion;
        std::list<KeyValuee> HttpHeaders;
        std::string Body;
        std::string Method;
        std::string Path;
        std::string Line;
        std::string filename;
        std::string data;
        std::string name;
        std::string remain;
        int Fd; bool flage;
    public:
        RequestParsser();
        RequestParsser(int Fd);
        ~RequestParsser();
        RequestParsser(const RequestParsser &copy);
        RequestParsser &operator=(const RequestParsser &src);

        void PrintHeaders() const;
        
        std::string GetHeader(std::string key) const;
        std::string GetHttpMethod() const;
        std::string GetPath() const;
        std::string GetBody() const;
        
        std::string Getfilename() const;
        std::string Getname() const;
        std::string GetData() const;
        std::string GetRemain() const;

        void ReadBody(int fd, const std::string &header);
        void ReadChunkedBody(int fd);
        bool GetFlage() const;
       
};


