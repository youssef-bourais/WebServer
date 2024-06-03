/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:39:55 by ybourais          #+#    #+#             */
/*   Updated: 2024/05/05 13:34:49 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include <cstdio>
#include <sys/_types/_s_ifmt.h>

std::string GetDayName(int n) 
{
    switch(n) 
    {
        case 0:
            return "Sunday";
        case 1:
            return "Monday";
        case 2:
            return "Tuesday";
        case 3:
            return "Wednesday";
        case 4:
            return "Thursday";
        case 5:
            return "Friday";
        case 6:
            return "Saturday";
        default:
            return "Invalid day of week";
    }
}

std::string GetMonthName(int n) 
{
    switch(n) 
    {
        case 0:
            return "January";
        case 1:
            return "February";
        case 2:
            return "March";
        case 3:
            return "April";
        case 4:
            return "May";
        case 5:
            return "June";
        case 6:
            return "July";
        case 7:
            return "August";
        case 8:
            return "September";
        case 9:
            return "October";
        case 10:
            return "November";
        case 11:
            return "December";
        default:
            return "Invalid month";
    }
}

std::string HttpResponse::GetHttpStatusMessage() const 
{ 
    switch(this->StatusCode) 
    {
        case HTTP_OK:
            return "OK";
        case HTTP_CREATED:
            return "Created";
        case HTTP_BAD_REQUEST:
            return "Bad Request";
        case HTTP_NOT_FOUND:
            return "Not Found";
        case HTTP_METHOD_NOT_ALLOWED:
            return "Method Not Allowed";
        case HTTP_INTERNAL_SERVER_ERROR:
            return "Internal Server Error";
        default:
            return "Unknown";
    }
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
        throw std::runtime_error(std::string("server: unable opening file:"));
    }
    char Resource[GetFileSize(FilePath) + 1];
    
    memset(Resource, 0, GetFileSize(FilePath) + 1);
    int r = read(fd, Resource, GetFileSize(FilePath));
    if(r == -1)
    {
        close(fd);
        throw std::runtime_error(std::string("server: unable reading file:"));
    }
    close(fd);
    return Resource;
}

int FileOrDir(std::string Path)
{
    struct stat s;
    std::string tmp = "." + Path;
    if(!stat(tmp.c_str(),&s))
    {
        if(s.st_mode & S_IFDIR)
            return 1;
        else if(s.st_mode & S_IFREG)
            return 0;
    }
    return -1;
}

#include <dirent.h>

std::string OpenDir(std::string Dir)
{
    DIR *dir;

    std::string tmp = "." + Dir;
    dir = opendir(tmp.c_str());

    if(!dir)
        throw std::runtime_error(std::string("server: unable to open directory:"));

    struct dirent *entry;

    std::string List;
    while((entry = readdir(dir)))
    {
        if(entry->d_name[0] != '.')
        {
            std::string newline = "\n";
            List += entry->d_name + newline;
            std::string PathOfReource = Dir + entry->d_name;
            if(FileOrDir(PathOfReource)) 
                printf("Dir: %s\n", entry->d_name);
            else 
                printf("File: %s\n", entry->d_name);
        }
    }
    closedir(dir);
    return List;
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
            int var = FileOrDir(Uri);
            if(var)
                Resource = OpenDir(Uri);
            else if(!var)
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


