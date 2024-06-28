/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:39:55 by ybourais          #+#    #+#             */
/*   Updated: 2024/06/28 13:07:57 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "../Tools/Tools.hpp"
#include <cstdio>
#include <string>

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

HttpResponse::HttpResponse(const HttpResponse& copy) : HttpMessage(copy) 
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
    if(FilePath.empty())
        return 0;
    if (access(FilePath.c_str(), F_OK) != -1) 
        return 1;
    return 0;
}

int GetFileSize(std::string Path)
{
    struct stat st;
    stat(Path.c_str(), &st);
    int FileSize = st.st_size;
    return FileSize;
}

std::string ReadFile(std::string FilePath)
{
    int fd;
    std::string FileToOpen = FilePath;
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

// int checkFileType(const std::string &path) 
// {
//     struct stat s;
//     std::string tmp = "." + path;
//     // std::cout << path<<std::endl;
//     // exit(0);
//     if (stat(path.c_str(), &s) == 0) 
//     {
//         if (s.st_mode & S_IFDIR) 
//             return 1;
//         else if (s.st_mode & S_IFREG) 
//             return 0;
//     }
//     return -1;
// }

#include <dirent.h>

std::string OpenDir(const std::string &Dir)
{
    DIR *dir;

    std::string tmp = Dir;
    dir = opendir(tmp.c_str());

    if(!dir)
        throw std::runtime_error(std::string("server: unable to open directory:"));

    struct dirent *entry;

    std::string List;
    while((entry = readdir(dir)))
    {
        std::string s = entry->d_name;
        if(s == "."|| s == "..")
            continue;
        List += entry->d_name;
        std::string PathOfReource = Dir + entry->d_name;
        if(checkFileType(PathOfReource) == DIR_TYPE) 
            List += "/";
        List +=  "\n";
    }
    closedir(dir);
    return List;
}

std::string generateListItems(const std::string &fileAndDirNames, const std::string &Uri) 
{
    std::string Items;
    std::istringstream Toread(fileAndDirNames);
    std::string Name;
    
    while (std::getline(Toread,Name)) 
    {
        std::string CompletPath = Uri + Name;
        Items += "<li><a href=\"" + Name + "\">" + Name + "</a></li>\n";
    }
    return Items;
}


std::string InitPage(const std::string &List, const std::string &Uri)
{
    std::string listContent = generateListItems(List, Uri);

    std::string htmlContent =
    "<!DOCTYPE html>\n"
    "<html lang=\"en\">\n"
    "<head>\n"
    "    <meta charset=\"UTF-8\">\n"
    "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
    "    <title>Directory Listing</title>\n"
    "    <style>\n"
    "        body {\n"
    "            font-family: Arial, sans-serif;\n"
    "            margin: 20px;\n"
    "        }\n"
    "        h1 {\n"
    "            margin-bottom: 20px;\n"
    "        }\n"
    "        ul {\n"
    "            list-style-type: none;\n"
    "            padding: 0;\n"
    "        }\n"
    "        li {\n"
    "            margin-bottom: 10px;\n"
    "        }\n"
    "        a {\n"
    "            text-decoration: none;\n"
    "            color: #007bff;\n"
    "        }\n"
    "    </style>\n"
    "</head>\n"
    "<body>\n"
    "    <h1>Directory Listing</h1>\n"
    "    <ul>\n"
            + listContent +
    "    </ul>\n"
    "</body>\n"
    "</html>\n";
    return htmlContent;
}

void ListDir(std::string &List, const std::string &Uri)
{
    std::string HtmlPage = InitPage(List, Uri);
    List = HtmlPage;
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
            int var = checkFileType(Uri);
            if(var == DIR_TYPE)
            {
                Resource = OpenDir(Uri);
                ListDir(Resource, Uri);// still need to work recurcivly
            }
            else if(var == FILE_TYPE)
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



/* std::string GetDate() */
/* { */
/*     std::time_t currentTime = std::time(NULL); */
/**/
/*     std::tm* timeinfo = std::gmtime(&currentTime); */
/**/
/*     std::stringstream ss; */
/**/
/*     ss << GetDayName(timeinfo->tm_wday)  << ", " */
/*        << timeinfo->tm_mday << " " */
/*        << GetMonthName(timeinfo->tm_mon) << " " */
/*        << (timeinfo->tm_year + 1900) << " " */
/*        << timeinfo->tm_hour << ":" */
/*        << timeinfo->tm_min << ":" */
/*        << timeinfo->tm_sec << " GMT"; */

    /* std::cout << "date: "<< ss.str()<<std::endl; */
    /* std::time_t currenttime = std::time(null); */
    /* std::tm *timeinfo = std::gmtime(&currenttime); */
    /* static char buffer[80]; */
    /**/
    /* std::strftime(buffer, sizeof(buffer), "date: %a, %d %b %y %h:%m:%s gmt", timeinfo); */

/*     return ss.str(); */
/* } */

#include <ctime>  

std::string GetDate() 
{
    time_t currentTime = std::time(NULL); // Correctly use time_t without std::
    tm* timeinfo = std::gmtime(&currentTime); // Correctly use tm and gmtime without std::

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
    return std::string(buffer);
}

std::string Getlenth(std::string File)
{
    std::stringstream ss;
    ss << GetFileSize(File);
    return  ss.str();
}

std::string ToString(int num)
{
    std::stringstream ss;
    ss << num;
    return  ss.str();
}

std::list<KeyValue> SetResponseHeaders(const HttpResponse &Response, const HttpRequest &Request)
{
    (void)Request;
    (void)Response;
    std::list<KeyValue> tmp;
    std::string key;
    std::string Value;

    key = "Server: ";
    Value = "Ragnar's";
    tmp.push_back(KeyValue(key, Value));

    key = "Conenection: ";
    Value = "close";
    tmp.push_back(KeyValue(key, Value));

    key= "Date: ";
    Value = GetDate();
    tmp.push_back(KeyValue(key, Value));

    if(!Response.GetResponseBody().empty())
    {
        key = "Content-Length: ";
        Value = ToString(Response.GetResponseBody().length());
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


