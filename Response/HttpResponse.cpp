/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-bah <sait-bah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:39:55 by ybourais          #+#    #+#             */
/*   Updated: 2024/07/16 00:51:39 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "../Tools/Tools.hpp"
#include <atomic>
#include <cstdio>
#include <string>
#include <sys/syslimits.h>

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
        case HTTP_MOVED_PERMANETLY:
            return "Moved Permanetly";

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
    {
        return 1;
    }
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
    std::cout << Uri<<std::endl;

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
    while (i < (int)uri.size()) 
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
    if(Request.GetHeader("Transfer-Encoding").empty() && Request.GetHeader("Content-Length").empty() && Request.GetHttpMethod() == "POST" && !Request.GetBody().empty())
    {
        Response.SetHTTPStatusCode(HTTP_BAD_REQUEST);
        return 0;
    }
    return 1;
}




std::string rootPath(std::string path, std::string root)
{
    std::string Path;
    if(root.size() == 1 && root == "/")
        root = "./";

    if(root[root.size() - 1] != '/' && path[0] != '/')
    {
        Path = root.substr(2) + "/" + path;
    }
    else 
    {
        Path = root.substr(2) + path;
    }
    return Path;
}



int LocationIsMatching(t_servers &ServerSetting, std::string &Path)
{
    std::string rootpath = "./var/www/html/";
    if(ServerSetting.root.empty())
    {
        ServerSetting.root = rootpath;
    }
    else 
    {
        rootpath = ServerSetting.root;
    }
    std::string realpath;
    
    realpath = rootPath(Path, rootpath);
    
    std::string tmp = Path;
    
    if(tmp.size() != 1)
    {
        tmp = tmp.substr(0, tmp.find("/") );
    }
    
    for (int i = 0;i < (int)ServerSetting.locations.size();i++) 
    {
        if(ServerSetting.locations[i].root.empty())
        {
            ServerSetting.locations[i].root = rootpath;
        }
         std::string tmp2;
        if(ServerSetting.locations[i].location.size() != 1)
            tmp2 = ServerSetting.locations[i].location.substr(2);
        else
            tmp2 = ServerSetting.locations[i].location;

        if(tmp2[tmp2.size() - 1] == '/' && tmp2.size() != 1)
        {
            tmp2 = tmp2.substr(0, tmp2.size() - 1);
        }
        int flage = 0;
        if(tmp.find(".") != std::string::npos && tmp2 == "/")
        {
            flage = 1;
        }
        
        if(tmp2 == tmp || flage)
        {
            std::string locationRoot = ServerSetting.locations[i].root;
            if(locationRoot[locationRoot.size() - 1] != '/')
            {
                locationRoot += "/";
            }
                std::string locationr;
                if(locationRoot != "/")
                {
                    locationr = locationRoot.substr(2);
                }
                else
                    locationr = "";
                
                int Index = Path.find("/");
                
                Path = locationr + Path.substr(Index + 1);
                return i;
        }
    }
   
    if(!CheckIfResourceExists(realpath))
    {
        return -1;
    }

    Path = realpath;
    return -2;
}

int File(std::string Path)
{
    struct stat s;
    std::string tmp = "./" + Path;
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

std::string HtmlToSring(std::string filePath)
{

    std::ifstream file(filePath.c_str());
    if (!file.is_open()) 
    {
        return "";
    }
    std::string content;
    std::string line;
    while (std::getline(file, line)) 
    {
        content += line;
        if (!file.eof()) 
        {
            content += "\n";
        }
    }

    file.close();
    return content;
}

std::string readHtmlPage(int index, t_servers ServerSetting) 
{
    if(ServerSetting.index.size() == 0)
    {
        ServerSetting.index.push_back("./HtmlPages/index.html");
    }

    std::string filePath;
    if(index == -2)
    {
        int i = 0;
        while (i < (int)ServerSetting.index.size()) 
        {
            if(CheckIfResourceExists(ServerSetting.index[i]))
            {
                filePath = ServerSetting.index[i];
            }
            else 
            {
                return "";
            }
            i++;
        }
    }
    else 
    {
        std::string tmp;
        if(ServerSetting.locations[index].index.empty())
        {
            for (int i = 0;i < (int)ServerSetting.index.size(); i++) 
            {
                if(CheckIfResourceExists(ServerSetting.index[i]))
                {
                    filePath = ServerSetting.index[i];
                    break;
                }
                else 
                {
                    return "";
                }
            }
        }
        else 
        {
            for (int i = 0;i < (int)ServerSetting.locations[index].index.size();i++) 
            {
                if(CheckIfResourceExists(ServerSetting.locations[index].index[i]))
                {
                    filePath = ServerSetting.locations[index].index[i];
                    break;
                }
                else 
                {
                    return "";
                }
            }
        }
    }
    std::string content = HtmlToSring(filePath);
    return content;
}

 int GetAutoIndex(t_servers ServerSetting, int index)
 {
    if(index == -2)
    {
        if(ServerSetting.index.size() == 0 && ServerSetting.autoIndex == true)
        {
            return 1; // dispaly dir
        }
        else if(ServerSetting.index.size() != 0) 
        {
            return 0; // read index page
        }
        else 
        {
            return -1; // Forbidden
        }
    }
    else 
    {
        if(ServerSetting.locations[index].index.size() == 0 && ServerSetting.locations[index].autoIndex == true)
        {
            return 1;
        }
        else if(ServerSetting.locations[index].index.size() != 0) 
        {
            return 0;
        }
        else 
        {
            if(ServerSetting.locations[index].index.empty())
            {

            }
            return -1;
        }
    }
     return 1;
 }

int IsMethodAllowed(t_servers ServerSetting, int index, std::string Method)
{
    if(ServerSetting.locations[index].allowedMethods.size() > 3 || ServerSetting.locations[index].allowedMethods.size() == 0)
    {
        ServerSetting.locations[index].allowedMethods.assign(ServerSetting.allowedMethods.begin(), ServerSetting.allowedMethods.end());
    }
    if(index == -2)
    {
        for (int i = 0; i < (int)ServerSetting.allowedMethods.size(); i++) 
        {
            if(Method == ServerSetting.allowedMethods[i])
            {
                return 1;
            }
        }
    }
    else 
    {
        for (int j = 0; j < (int)ServerSetting.locations[index].allowedMethods.size(); j++) 
        {
            if(Method == ServerSetting.locations[index].allowedMethods[j])
            {
                return 1;
            }
        }
    
    }
    return 0;
}

int GetMaxBodySize(t_servers ServerSetting, int index)
{
    if(ServerSetting.maxBodySize.empty())
    {
        ServerSetting.maxBodySize = "100";
    }
    if(index == -2)
    {
        return StringToInt(ServerSetting.maxBodySize);
    }
    else 
    {
        if(ServerSetting.locations[index].maxBodySize.empty())
        {
            ServerSetting.locations[index].maxBodySize = ServerSetting.maxBodySize;
        }
        return StringToInt(ServerSetting.locations[index].maxBodySize);
    }
    return 0;
}

std::string err_pages(t_servers ServerSetting, int index, HTTPStatusCode StatusCode)
{
    std::cout << ServerSetting.locations[index].errPage[0]<<std::endl;
    // std::cout << ServerSetting.jjjll<<std::endl;
    return "";
}

std::string GetResource(const RequestParsser &Request, HttpResponse &Response, t_servers &ServerSetting)
{
    std::string Resource;
    std::string Method = Request.GetHttpMethod();
    std::string uri = Request.GetPath();



    // int MaxBodySize = StringToInt(ServerSetting.maxBodySize);

    if(!IsRequestGood(Request, Response))
    {
        return "";
    }

    std::string &Uri = uri;
    int index = LocationIsMatching(ServerSetting, Uri);
    if(index == -1)
    {
        Response.SetHTTPStatusCode(HTTP_NOT_FOUND);
        return "";
    }
    if(Request.GetPath() == "/")
    {
        Resource = readHtmlPage(index, ServerSetting);
        if(Resource.empty())
        {
            Response.SetHTTPStatusCode(HTTP_NOT_FOUND);
            return "";
        }
        Response.SetHTTPStatusCode(HTTP_OK);
        return Resource;
    }
    if(!ServerSetting.locations[index].redirect.empty() && index >= 0)
    {
        Response.SetHTTPStatusCode(HTTP_MOVED_PERMANETLY);
        Response.SetConfigNum(index);
        return "";
    }

    int autoIndex = GetAutoIndex(ServerSetting, index);
    int allowedMethod = IsMethodAllowed(ServerSetting, index, Method);
    int maxBodySize = GetMaxBodySize(ServerSetting, index);
    
    if(Method == "GET" && allowedMethod == 1)
    {
        int var = checkFileType(Uri);
        if(var == DIR_TYPE)
        {
            if(autoIndex == 1)
            {
                Resource = OpenDir(Uri);
                ListDir(Resource, Uri);
                Response.SetHTTPStatusCode(HTTP_OK);
                return Resource;
            }
            else if(autoIndex == 0)
            {
                Resource = readHtmlPage(index, ServerSetting);
                return Resource;
            }
            else
            {
                Response.SetHTTPStatusCode(HTTP_FORBIDDEN);
                return "";
            }
        }
        else if(var == FILE_TYPE)
        {
            Resource = ReadFile(Uri);
            Response.SetHTTPStatusCode(HTTP_OK);
        }
        else 
        {
            Response.SetHTTPStatusCode(HTTP_NOT_FOUND);
            return "";
        }
    }
    else if(Method == "POST" && allowedMethod == 1)
    {
        if((int)Request.GetBody().size() > maxBodySize)
        {
            Response.SetHTTPStatusCode(HTTP_ENTITY_TOO_LARGE);
            return "";
        }
        Response.SetHTTPStatusCode(HTTP_OK);
    }
    else if(Method == "DELETE" && allowedMethod == 1)
    {

        Response.SetHTTPStatusCode(HTTP_OK);
        Delete(Request, Response, ServerSetting);
        std::cout << "delete"<<std::endl;
    }
    else 
    {
        Response.SetHTTPStatusCode(HTTP_METHOD_NOT_ALLOWED);
    }
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




std::list<KeyValue> SetResponseHeaders(const HttpResponse &Response, const RequestParsser &Request, t_servers &ServerSetting, int index)
{
    (void)Request;
    (void)Response;
    std::list<KeyValue> tmp;
    std::string key;
    std::string Value;

    if(Response.GetStatusCode() == HTTP_MOVED_PERMANETLY && !ServerSetting.locations[index].redirect.empty())
    {
        key = "Location: ";
        Value = ServerSetting.locations[index].redirect;
        tmp.push_back(KeyValue(key, Value));
    }

    key = "Server: ";
    Value = ServerSetting.server_names[0];
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

    key = "Conenection: ";
    Value = "close";
    tmp.push_back(KeyValue(key, Value));

    return tmp;
}

void HttpResponse::SetConfigNum(int index)
{
    this->Confignum = index;
}

HttpResponse::HttpResponse(const RequestParsser &Request, t_servers &ServerSetting)
{
    this->Confignum = 0;
    this->Request = Request;
    this->ServerSetting = ServerSetting;
    this->ResponseBody = GetResource(Request, *this, ServerSetting);
    this->ResponseHeaders = SetResponseHeaders(*this, Request, ServerSetting, this->Confignum);
}


