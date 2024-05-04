/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:39:55 by ybourais          #+#    #+#             */
/*   Updated: 2024/05/04 15:27:37 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

void HttpResponse::SetHTTPStatusCode(HTTPStatusCode Code)
{
    this->StatusCode = Code;
}

std::list<KeyValue>::const_iterator HttpResponse::GetHeadersEnd() const
{
    return this->ResponseHeaders.end();
}

std::list<KeyValue>::const_iterator HttpResponse::GetHeadersBegin() const
{
    return this->ResponseHeaders.begin();
}

std::string HttpResponse::HTTPStatusCodeToString() const 
{
    std::ostringstream oss;
    oss << this->StatusCode;
    return oss.str();
}

int CheckIfResourceExists(std::string FilePath)
{
    std::string tmp = "." + FilePath;
    if (access(tmp.c_str(), F_OK) != -1) 
        return 1;
    return 0;
}

int GetFileSize(std::string Path)
{
    struct stat st;
    std::string tmp = "." + Path;
    stat(tmp.c_str(), &st);
    int FileSize = st.st_size;
    return FileSize;
}

std::string ReadFile(std::string FilePath)
{
    int fd;
    std::string FileToOpen = "." + FilePath;
    if((fd = open(FileToOpen.c_str(), O_RDONLY)) == -1)
    {
        close(fd);
        throw std::runtime_error(std::string("server: opening file:"));
    }
    char Resource[GetFileSize(FilePath) + 1];
    
    memset(Resource, 0, GetFileSize(FilePath) + 1);
    int r = read(fd, Resource, GetFileSize(FilePath));
    if(r == -1)
    {
        close(fd);
        throw std::runtime_error(std::string("server: reading file:"));
    }
    close(fd);
    return Resource;
}


std::string OpenDir()
{
    return "Not listing Dir Yet";
}


int FileOrDir(std::string Path)
{
    struct stat s;
    if( stat(Path.c_str(),&s) == 0)
    {
        if(s.st_mode & S_IFDIR)
            return 1;
        else if(s.st_mode & S_IFREG)
            return 0;
    }
    return -1;
}


std::string GetResource(const HttpRequest &Request, HttpResponse &Response)
{
    std::string Resource;
    std::string Method = Request.GetHttpMethod();
    std::string Uri = Request.GetPath();
    
    if(Method == "GET")
    {
        if(CheckIfResourceExists(Uri) && Uri != "/")
        {
            if(FileOrDir(Uri))
                Resource = OpenDir();
            else if(!FileOrDir(Uri))
                Resource = ReadFile(Uri);
        }
        else 
            Response.SetHTTPStatusCode(HTTP_NOT_FOUND);
    }
    else if(Method == "POST")
    {

    }
    else if(Method == "DELETE")
    {

    }
    else 
        Response.SetHTTPStatusCode(HTTP_METHOD_NOT_ALLOWED);
    return Resource;
}

std::string HttpResponse::GetResponseBody() const
{
    return this->ResponseBody;
}

HTTPStatusCode HttpResponse::GetStatusCode() const
{
    return this->StatusCode;
}

HTTPStatusCode GetHttpStatusCode(const HttpRequest &Request, const HttpResponse &response)
{
    (void)Request;
    (void)response;

    return HTTP_OK;
}



std::string GetDate()
{
    std::time_t currentTime = std::time(NULL);

    std::tm* timeinfo = std::gmtime(&currentTime);

    std::stringstream ss;

    ss << GetDayName(timeinfo->tm_wday)  << ", "
       << timeinfo->tm_mday << " "
       << GetMonthName(timeinfo->tm_mon) << " "
       << (timeinfo->tm_year + 1900) << " "
       << timeinfo->tm_hour << ":"
       << timeinfo->tm_min << ":"
       << timeinfo->tm_sec << " GMT";

    /* std::cout << "date: "<< ss.str()<<std::endl; */
    /* std::time_t currenttime = std::time(null); */
    /* std::tm *timeinfo = std::gmtime(&currenttime); */
    /* static char buffer[80]; */
    /**/
    /* std::strftime(buffer, sizeof(buffer), "date: %a, %d %b %y %h:%m:%s gmt", timeinfo); */

    return ss.str();
}

std::string Getlenth(std::string File)
{
    std::stringstream ss;
    ss << GetFileSize(File);
    return  ss.str();
}

std::list<KeyValue> SetResponseHeaders(const HttpResponse &Response, const HttpRequest &Request)
{
    std::list<KeyValue> tmp;
    std::string key;
    std::string Value;

    key = "Server: ";
    Value = "Ragnar's";
    tmp.push_back(KeyValue(key, Value));
    
    key= "Date: ";
    Value = GetDate();
    tmp.push_back(KeyValue(key, Value));
    
    if(!Response.GetResponseBody().empty())
    {
        key = "Content-Length: ";
        Value = Getlenth(Request.GetPath());
        tmp.push_back(KeyValue(key, Value));
    }
        
    key = "Allow: ";
    Value = "GET, POST, DELETE";
    tmp.push_back(KeyValue(key, Value));

    return tmp;
}

HttpResponse::HttpResponse(const HttpRequest &Request) : HttpMessage(Request.GetRecivedLine())
{
    this->StatusCode = GetHttpStatusCode(Request, *this);
    this->ResponseBody = GetResource(Request, *this);
    this->ResponseHeaders = SetResponseHeaders(*this, Request);
}

HttpResponse::~HttpResponse()
{

}

HttpResponse &HttpResponse::operator=(const HttpResponse &s) 
{
    if(this != &s)
    {
    }
    return *this;
}

HttpResponse::HttpResponse(const HttpResponse& copy) 
{
    *this = copy;
}
