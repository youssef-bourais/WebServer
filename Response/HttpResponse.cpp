/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-bah <sait-bah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:39:55 by ybourais          #+#    #+#             */
/*   Updated: 2024/07/17 22:26:19 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "../Tools/Tools.hpp"
#include <atomic>
#include <cerrno>
#include <cstdio>
#include <iterator>
#include <ostream>
#include <random>
#include <string>
#include <sys/syslimits.h>

#include <signal.h>
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

HttpResponse::HttpResponse(const HttpResponse& copy): Request(copy.Request) 
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


int IsRequestGood(const RequestParsser &Request, HttpResponse &Response, HTTPStatusCode &code)
{

    std::string Uri = Request.GetPath();
    
    if(hasDisallowedChars(Uri))
    {
        Response.SetHTTPStatusCode(HTTP_BAD_REQUEST);
        code = HTTP_BAD_REQUEST;
        return 0;
    }
    if(Uri.size() > 2048)
    {
        Response.SetHTTPStatusCode(HTTP_URI_TOO_LONG);
        code = HTTP_URI_TOO_LONG;
        return 0;
    }
    if(!Request.GetHeader("Transfer-Encoding").empty() && Request.GetHeader("Transfer-Encoding") != "chunked")
    {
        Response.SetHTTPStatusCode(HTTP_NOT_IMPLEMENTED);
        code = HTTP_NOT_IMPLEMENTED;
        return 0;
    }
    if(Request.GetHeader("Transfer-Encoding").empty() && Request.GetHeader("Content-Length").empty() && Request.GetHttpMethod() == "POST" && !Request.GetBody().empty())
    {
        Response.SetHTTPStatusCode(HTTP_BAD_REQUEST);
        code = HTTP_BAD_REQUEST;
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

int LocationIsMatching(t_servers &ServerSetting, std::string &Path, std::string Method)
{

    // if(Path.find("?") != std::string::npos)
    // {
    //
    // }
        
    std::string rootpath = "./var/html/www/";
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
        
         // std::cout << "location: "<<tmp2<<std::endl;
         // std::cout << "path: "<< tmp<<std::endl;

        if(tmp2 == tmp || flage)
        {
            if(Method == "POST")
                return i;
            if(Method == "GET" && tmp2.find(".py") != std::string::npos)
            {
                return i;
            }
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
    // if()
   
    if(!CheckIfResourceExists(realpath) && Method != "POST" && realpath.find(".py") == std::string::npos) 
    {
        return -1;
    }
    if(Method != "POST")
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



std::string GETMethod(t_servers ServerSetting, int index, std::string Uri, int autoIndex, HttpResponse &Response)
{
    std::string errPage;
    std::string Resource;








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

            errPage = err_pages(ServerSetting, index, HTTP_FORBIDDEN);
            return errPage;
        }
    }
    else if(var == FILE_TYPE || Uri.find(".py") != std::string::npos)
    {
        std::string file;

        // std::cout << "uri : "<<Uri<<std::endl;
        // exit(0);
        if(Uri.find(".py") != std::string::npos)// find .py
        {

            std::string root;
            if(index == -2)
            {
                std::string py = Uri.substr(ServerSetting.root.size() - 2);
                if(ServerSetting.cgiPath.empty())
                {
                    ServerSetting.cgiPath = "/pages/cgi/";
                    file = ServerSetting.cgiPath;
                }
                else 
                {
                    file = ServerSetting.cgiPath;
                }
                Uri = ServerSetting.root + file + py;
                std::cout << Uri<<std::endl;
                exit(0);
            }
            else 
            {
            
            }

        }
        else 
        {
        
            Resource = ReadFile(Uri);
            Response.SetHTTPStatusCode(HTTP_OK);
            return Resource;
        }
    }
    else 
    {
        Response.SetHTTPStatusCode(HTTP_NOT_FOUND);

        errPage = err_pages(ServerSetting, index, HTTP_NOT_FOUND);
        return errPage;
    }

    return Resource;
}

 char* str_dup(const std::string& str)
{
    char* res = new char[str.size() + 1];
    std::strcpy(res, str.c_str());
    res[str.size()] = '\0'; // Ensure null-termination
    return res;
}
std::string cgi_handler(RequestParsser &Request, std::string fullpath,std::string method)
{

    std::string body ;
    std::string query = "";
    if(method == "POST" && !Request.GetBody().empty())
    {
        body = Request.GetBody();
        int i = body.find(".py");
        body = body.substr(i + 3);

    }
    //std::string query = body;
    std::string cgi_path = "/usr/bin/python3";
    std::string name = fullpath;

    char *env[5];
    env[0] = str_dup("GATEWAY_INTERFACE=CGI/1.1");
    std::string  str= "REQUEST_METHOD=" + method;
    env[1] = str_dup((char *)str.c_str());
    if(method == "GET")
    {
	    query = "QUERY_STRING=" + query; //fro get
	    env[2] = str_dup((char*)query.c_str());
    }
    else if(method == "POST")
    {
        std::string bodyLen = "CONTENT_LENGTH="+intToString(body.size());
        env[2] = str_dup((char *)bodyLen.c_str());
    }
	std::string scriptFileName = "SCRIPT_FILENAME=" + name;
	env[3] = str_dup((char *)scriptFileName.c_str());
	env[4] = NULL;  
    std::string response;
    int status;
    int fdin[2];
    pid_t pid;
    int start = time(0);

    if (pipe(fdin) == -1) {
        perror("pipe");
        exit(1);
    }
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0)
    {
        if (method == "POST") //  we have post  
        {
            if (dup2(fdin[0], 0) == -1)
            {
                perror("dup2");
                exit(1);
            }
        }
        if (dup2(fdin[1], 1) == -1)
        {
            perror("dup2");
            exit(1);
        }

        close(fdin[0]);
        close(fdin[1]);

        char **av = new char *[3];
        av[0] = new char[cgi_path.length() + 1];
        av[1] = new char[std::strlen(name.c_str()) + 1];
        av[2] = NULL;
        std::strcpy(av[0], cgi_path.c_str());
        std::strcpy(av[1], name.c_str());
        if (execve(cgi_path.c_str(), av, env) == -1) // set env instance of NULL
        {
            perror("execve");
            exit(127);
        }
        exit(0);
    }
    else
    {
        if (method == "POST")
            write(fdin[1], body.c_str(), body.length());
        int result;
        while ((result = waitpid(pid, &status, WNOHANG)) == 0)
        {
            if (result == 0)
            {
                if ((time(0) - start) > 4)
                {
                    response = "HTTP/1.1 500 Internal Server Error";
                    kill(pid, SIGKILL);
                    close(fdin[1]);
                    return response;
                }
                sleep(1);
            }
        }
        if (WEXITSTATUS(status) != 0)
        {
            return "";
        }
        close(fdin[1]);
        char buffer[1024];

        while (true)
        {
            int len = read(fdin[0], buffer, 1023);
            if (len <= 0)
                break;
            if (len == 1023)
                buffer[len] = '\0';
            else
                buffer[len + 1] = '\0';
            if (len >= 0)
            {
                buffer[len] = '\0';
                response += buffer;
            }
        }
        return response;
    }
    return "";
}
#include <map>

std::map<std::string, std::string> QueryString(std::string url) 
{
    std::map<std::string, std::string> queryParams;

    std::size_t pos = url.find('?');
    if (pos == std::string::npos) 
    {
        return queryParams;
    }

    std::string queryString = url.substr(pos + 1);

    std::istringstream queryStream(queryString);
    std::string keyValue;
    while (std::getline(queryStream, keyValue, '&')) 
    {
        std::size_t eqPos = keyValue.find('=');
        if (eqPos != std::string::npos) 
        {
            std::string key = keyValue.substr(0, eqPos);
            std::string value = keyValue.substr(eqPos + 1);
            queryParams[key] = value;
        }
    }
    return queryParams;
}

void saveFile(std::string path, std::string fileName, std::string data) 
{
    std::ofstream out(path + fileName, std::ios::binary);
    out.write(data.data(), data.size());
    out.close();
}

std::string Post(t_servers ServerSetting, int index, std::string Uri, HttpResponse &Response, bool autoIndex, RequestParsser &Request)
{

    std::string content_type = Request.GetHeader("Content-Type");

    std::string script;

    std::map<std::string, std::string> querystring = QueryString(Uri);
    if(!querystring.empty())
    {


    }
    
    std::string res;
    std::string upload_dir;

   if(content_type.find("multipart/form-data") != std::string::npos && Uri.find(".py") == std::string::npos)
    {
    
        std::string file_name = Request.Getfilename();
        std::string data = Request.GetData();
        // this block working acourding the configfile
        if(index == -2)
        {
            if(ServerSetting.uploadPath.empty())
            {
                res = GETMethod(ServerSetting, index, Uri, autoIndex,Response);
                return res;
            }
            else 
            {
                if(ServerSetting.root.empty())
                {
                    ServerSetting.root = "./var/html/www/";
                }
                upload_dir = ServerSetting.root;
                if(ServerSetting.root[ServerSetting.root.size() - 1] != '/')
                    upload_dir += '/';
                upload_dir += ServerSetting.uploadPath.substr(2);
                if(upload_dir[upload_dir.size() - 1] != '/')
                    upload_dir += '/';

                saveFile(upload_dir, file_name, data);
                Response.SetHTTPStatusCode(HTTP_CREATED);
                return "";
            }
        }
        else 
        {
            if(ServerSetting.locations[index].uploadPath.empty())
            {
                res = GETMethod(ServerSetting, index, Uri, autoIndex,Response);
                return res;
            }
            else 
            {
                if(ServerSetting.locations[index].root.empty())
                {
                    if(ServerSetting.root.empty())
                        ServerSetting.locations[index].root = "./var/html/www/";
                    else
                        ServerSetting.locations[index].root = ServerSetting.root;
                }
                upload_dir = ServerSetting.locations[index].root;
                if(ServerSetting.locations[index].root[ServerSetting.locations[index].root.size() - 1] != '/')
                    upload_dir += '/';
                upload_dir += ServerSetting.locations[index].uploadPath.substr(2);

                if(upload_dir[upload_dir.size() - 1] != '/')
                    upload_dir += '/';

                saveFile(upload_dir, file_name, data);
                Response.SetHTTPStatusCode(HTTP_CREATED);
                return "";
            }
        }
    }
   else  // NOLINT
   {
           bool Cgi = false;

        std::string full_path;

        if(index == -2)
        {
            if(!ServerSetting.cgiExtentions.empty() && !ServerSetting.cgiPath.empty())
            {
                Cgi = true;
                if(ServerSetting.root.empty())
                    ServerSetting.root = "./var/html/www/";
                if(ServerSetting.cgiPath[0] == '.' && ServerSetting.cgiPath[1] == '/')
                {
                    ServerSetting.cgiPath = ServerSetting.cgiPath.substr(2);
                }
            }
            else 
            {
                Cgi = false;
                Response.SetHTTPStatusCode(HTTP_FORBIDDEN);
                return "";
            }
            full_path = ServerSetting.root + ServerSetting.cgiPath + Uri;
        }
        else 
        {
            if(!ServerSetting.locations[index].cgiExtentions.empty() && !ServerSetting.locations[index].cgiPath.empty())
            {
                Cgi = true;
                if(ServerSetting.locations[index].root.empty())
                {
                    ServerSetting.locations[index].root = "./var/html/www/";
                }
                if(ServerSetting.locations[index].cgiPath[0] == '.' && ServerSetting.locations[index].cgiPath[1] == '/')
                {
                    ServerSetting.locations[index].cgiPath = ServerSetting.locations[index].cgiPath.substr(2);
                }
            }
            else 
            {
                Response.SetHTTPStatusCode(HTTP_FORBIDDEN);
                return "";
            }

            Uri = Uri.substr(Uri.find("/") + 1);
            full_path = ServerSetting.locations[index].root + ServerSetting.locations[index].cgiPath + Uri;

        }

        struct stat Stat;
        // std::cout << full_path<<std::endl;
        if (stat(full_path.c_str(), &Stat) != 0)
        {
            // std::cout << "full_path: "<<full_path<<std::endl;
            // std::cout << "uri "<< Uri<<std::endl;
            // std::cout << Request.GetBody() << std::endl;
            // exit(0);

            Response.SetHTTPStatusCode(HTTP_NOT_FOUND);
            return err_pages(ServerSetting, index, HTTP_NOT_FOUND);
        }
        else if(S_ISDIR(Stat.st_mode))// check if the file exists
        {
            res = GETMethod(ServerSetting, index, full_path, autoIndex,Response);
            return res;
        }
            
        if (Uri.find(".py") != std::string::npos)
        {
    
            std::string name;
            std::string cgi_path;
            
            // std::cout << Request.GetBody()<<std::endl;
            res = cgi_handler(Request, full_path, "POST");
            if(res == "")
            {
                return err_pages(ServerSetting, index, HTTP_INTERNAL_SERVER_ERROR);
            }
            else 
            {
                Response.SetHTTPStatusCode(HTTP_OK);               return res;
            }
        }
        else if(Uri.find(".py") == std::string::npos)
        {
            res = GETMethod(ServerSetting, index, Uri, false,Response);
        }
    }
    return res;
}

std::string RequestParsser::GetRemain() const
{
    return this->remain;

}

std::string GetResource(RequestParsser &Request, HttpResponse &Response, t_servers &ServerSetting)
{
    std::string Resource;
    std::string Method = Request.GetHttpMethod();
    std::string uri = Request.GetPath();

    std::string errPage;
    HTTPStatusCode code = HTTP_OK;


    if(!IsRequestGood(Request, Response, code))
    {
        errPage = err_pages(ServerSetting, -2, code);
        return errPage;
    }

    std::string &Uri = uri;
    int index = LocationIsMatching(ServerSetting, Uri, Method);
    if(index == -1)
    {
        Response.SetHTTPStatusCode(HTTP_NOT_FOUND);
        
        errPage = err_pages(ServerSetting, -2, HTTP_NOT_FOUND);
        return errPage;

    }
    if(Request.GetPath() == "/")
    {
        Resource = readHtmlPage(index, ServerSetting);
        if(Resource.empty())
        {
            Response.SetHTTPStatusCode(HTTP_NOT_FOUND);
            
            errPage = err_pages(ServerSetting, index, HTTP_NOT_FOUND);
            return errPage;

        }
        Response.SetHTTPStatusCode(HTTP_OK);
        return Resource;
    }
    if(index >= 0 && !ServerSetting.locations[index].redirect.empty())
    {
        Response.SetHTTPStatusCode(HTTP_MOVED_PERMANETLY);
        Response.SetConfigNum(index);

        errPage = err_pages(ServerSetting, index, HTTP_MOVED_PERMANETLY);
        return errPage;
    }

    int autoIndex = GetAutoIndex(ServerSetting, index);
    int allowedMethod = IsMethodAllowed(ServerSetting, index, Method);
    int maxBodySize = GetMaxBodySize(ServerSetting, index);

    
    if(Method == "GET" && allowedMethod == 1)
    {
        Resource = GETMethod(ServerSetting, index, Uri, autoIndex, Response);
        return Resource;
    }
    else if(Method == "POST" && allowedMethod == 1)
    {
        if((int)Request.GetBody().size() > maxBodySize)
        {
            Response.SetHTTPStatusCode(HTTP_ENTITY_TOO_LARGE);
            errPage = err_pages(ServerSetting, index, HTTP_ENTITY_TOO_LARGE);
            return errPage;

        }
        return Post(ServerSetting, index, Uri, Response, autoIndex, Request);
    }

    else if(Method == "DELETE" && allowedMethod == 1)
    {
        Resource = Delete(Request, Response, ServerSetting, index, Uri);
        return Resource;
    }
    else 
    {
        Response.SetHTTPStatusCode(HTTP_METHOD_NOT_ALLOWED);
        errPage = err_pages(ServerSetting, index, HTTP_METHOD_NOT_ALLOWED);
        
        return errPage;

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

HttpResponse::HttpResponse(RequestParsser &Request, t_servers &ServerSetting): Request(Request)
{
    this->Confignum = 0;
    this->Request = Request;
    this->ServerSetting = ServerSetting;
    this->ResponseBody = GetResource(Request, *this, ServerSetting);
    this->ResponseHeaders = SetResponseHeaders(*this, Request, ServerSetting, this->Confignum);
}


