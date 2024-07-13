/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:39:55 by ybourais          #+#    #+#             */
/*   Updated: 2024/07/13 03:10:52 by ybourais         ###   ########.fr       */
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
        case HTTP_URI_TOO_LONG:
            return "Uri Too Long";
        case HTTP_ENTITY_TOO_LARGE:
            return "Entity Too Large";
        case HTTP_NOT_IMPLEMENTED:
            return "Not Implemented";
        case HTTP_FORBIDDEN:
            return "Forbidden";
        case HTTP_CONFLICT:
            return  "Conflict";
        case  HTTP_NO_CONTENT:
            return "No Content";

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
    int size = GetFileSize(FilePath);
    char* buffer = new char[size + 1];
    /* char Resource[size + 1]; */
    memset(buffer, 0, size + 1);
    int r = read(fd, buffer, size);
    if(r == -1)
    {
        close(fd);
        throw std::runtime_error(std::string("server: unable reading file:"));
    }
    buffer[r] = 0;
    close(fd);
    std::string content(buffer);

    delete[] buffer;
    return content;
}

int FileOrDir(std::string Path)
{
    struct stat s;
    std::string tmp = "." + Path;
    if(!stat(tmp.c_str(),&s))
    {
    
        std::cout << "size using stat: "<<s.st_size<<std::endl;
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

bool hasDisallowedChars(const std::string& uri) 
{
    const std::string allowedChars =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~:/?#[]@!$&'()*+,;=";

    int i = 0;
    while (i < uri.size()) 
    {
        if (allowedChars.find(uri[i]) == std::string::npos) 
        {
            return true; // Disallowed character found
        }
        i++;
    }
    return false;
}


int IsRequestGood(const RequestParsser &Request, HttpResponse &Response)
{

    std::string Uri = Request.GetPath();
    
    if(hasDisallowedChars(Uri))
    {
        Response.SetHTTPStatusCode(HTTP_BAD_REQUEST);
        return 0;
    }
    if(Uri.size() > 2048)
    {
        Response.SetHTTPStatusCode(HTTP_URI_TOO_LONG);
        return 0;
    }
    if(!Request.GetHeader("Transfer-Encoding").empty() && Request.GetHeader("Transfer-Encoding") != "chunked")
    {
        Response.SetHTTPStatusCode(HTTP_NOT_IMPLEMENTED);
        return 0;
    }
    if(Request.GetHeader("Transfer-Encoding").empty() && Request.GetHeader("Content-Length").empty() && Request.GetHttpMethod() == "POST")
    {
        Response.SetHTTPStatusCode(HTTP_BAD_REQUEST);
        return 0;
    }
    return 1;
}




int LocationIsMatching(t_servers ServerSetting, std::string Path)
{
    if(!CheckIfResourceExists(Path))
        return 0;

    std::string n = Path.substr(0, Path.find("/"));
    std::cout << "k: "<<n<<std::endl;

    std::string tmp = "/";
    tmp += Path;
    for (int i = 0;i < ServerSetting.locations.size();i++) 
    {
        // if()
        // {

        // }
         std::cout << "path: "<<ServerSetting.locations[i].location<<std::endl;
    }
    return 1;
}

std::string GetResource(const RequestParsser &Request, HttpResponse &Response, t_servers &ServerSetting)
{
    std::string Resource;
    std::string Method = Request.GetHttpMethod();
    std::string Uri = Request.GetPath();

    int MaxBodySize = StringToInt(ServerSetting.maxBodySize);

    if(!IsRequestGood(Request, Response))
        return "";

    // if(!LocationIsMatching(ServerSetting, Uri))
    // {
    //     Response.SetHTTPStatusCode(HTTP_NOT_FOUND);
    //     return "";
    // }
        
    if(Method == "GET")
    {
        if(CheckIfResourceExists(Uri) && Uri != "/")
        {
            int var = checkFileType(Uri);
            if(var == DIR_TYPE)
            {
                if(ServerSetting.autoIndex)
                {
                    Resource = OpenDir(Uri);
                    ListDir(Resource, Uri);// still need to work recurcivly
                    Response.SetHTTPStatusCode(HTTP_OK);
                }
                else
                    Response.SetHTTPStatusCode(HTTP_FORBIDDEN);
            }
            else if(var == FILE_TYPE)
            {
                Resource = ReadFile(Uri);
                Response.SetHTTPStatusCode(HTTP_OK);
            }
        }
        else if(Uri == "/")
        {
            Response.SetHTTPStatusCode(HTTP_OK);
        }
        else 
        {
            Response.SetHTTPStatusCode(HTTP_NOT_FOUND);
        }

    }
    else if(Method == "POST")
    {
        if(StringToInt(ServerSetting.maxBodySize) < Request.GetBody().size() )
        {
            Response.SetHTTPStatusCode(HTTP_ENTITY_TOO_LARGE);
        }
    }
    else if(Method == "DELETE")
    {

        Delete(Request, Response);
        std::cout << "delete"<<std::endl;

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



std::string GetDate() 
{
    time_t currentTime = std::time(NULL);
    tm* timeinfo = std::gmtime(&currentTime);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
    return std::string(buffer);
}




std::list<KeyValue> SetResponseHeaders(const HttpResponse &Response, const RequestParsser &Request, t_servers &ServerSetting)
{
    (void)Request;
    (void)Response;
    std::list<KeyValue> tmp;
    std::string key;
    std::string Value;

    key = "Server: ";
    Value = ServerSetting.server_names[0];
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
        Value = intToString(Response.GetResponseBody().length());
        tmp.push_back(KeyValue(key, Value));
    }
    key = "Allow: ";
    Value = "GET, POST, DELETE";
    tmp.push_back(KeyValue(key, Value));

    return tmp;
}

HttpResponse::HttpResponse(const RequestParsser &Request, t_servers &ServerSetting)
{
    this->Request = Request;
    this->ServerSetting = ServerSetting;
    this->ResponseBody = GetResource(Request, *this, ServerSetting);
    this->ResponseHeaders = SetResponseHeaders(*this, Request, ServerSetting);
}



