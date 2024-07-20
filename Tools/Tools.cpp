

#include "Tools.hpp"
#include <atomic>

int checkFileType(const std::string &path) 
{
    struct stat s;
    if (stat(path.c_str(), &s) == 0) 
    {
        if (s.st_mode & S_IFDIR)
            return DIR_TYPE;
        else if (s.st_mode & S_IFREG)
            return FILE_TYPE;
    }
    return UNKNOWN_TYPE;
}


void ImOut(std::string msg)
{
    std::cout << msg<<std::endl;
    exit(0);
}

void printVect(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, std::string Key)
{
    std::cout << Key<<": ";
    while(begin != end)
    {
        std::cout << *begin<<", ";
        begin ++;
    }
    std::cout <<std::endl;
}

void printConfigFile(std::string Path)
{
    Parsing parse(Path);
    std::vector<t_servers> ServerSetting = parse.getServers();
    
    int i = 0;
    while(i < (int)ServerSetting.size())
    {
        std::cout << "Server++++++"<<std::endl;
        printVect(ServerSetting[i].listen.begin(), ServerSetting[i].listen.end(), "listen");
        std::cout << "Host: " + ServerSetting[i].host<<std::endl;
        printVect(ServerSetting[i].server_names.begin(), ServerSetting[i].server_names.end(), "server_names");
        std::cout << "maxBodySize: " + ServerSetting[i].maxBodySize<<std::endl;
        printVect(ServerSetting[i].allowedMethods.begin(), ServerSetting[i].allowedMethods.end(), "allowedMethods");
        printVect(ServerSetting[i].index.begin(), ServerSetting[i].index.end(), "index");
        std::cout << "error_pages: " + ServerSetting[i].errPage<<std::endl;
        std::cout << "autoIndex: " << ServerSetting[i].autoIndex<<std::endl;
        std::cout << "root: " + ServerSetting[i].root<<std::endl;

        int j = 0;
        while(j < (int)ServerSetting[i].locations.size())
        {
            std::cout << "Locations++++++"<< std::endl;

            std::cout << "location: " + ServerSetting[i].locations[j].location<<std::endl;
            std::cout << "root: "<<ServerSetting[i].locations[j].root << std::endl;
            std::cout << "autoindex: "<<ServerSetting[i].locations[j].autoIndex << std::endl;
            printVect(ServerSetting[i].locations[j].index.begin(), ServerSetting[i].locations[j].index.end(), "index");
            std::cout << "maxBodySize: "<<ServerSetting[i].locations[j].maxBodySize << std::endl;
            std::cout << "errPage: "<<ServerSetting[i].locations[j].errPage << std::endl;
            printVect(ServerSetting[i].locations[j].allowedMethods.begin(), ServerSetting[i].locations[j].allowedMethods.end(), "allowedMethods");
            
            std::cout << "proxy_pass: "<<ServerSetting[i].locations[j].proxyPass<<std::endl;
            std::cout << "cgiExtentions: "<<ServerSetting[i].locations[j].cgiExtentions<<std::endl;
            std::cout << "cgiPath: "<<ServerSetting[i].locations[j].cgiPath<<std::endl;
            std::cout << "uploadPath: "<<ServerSetting[i].locations[j].uploadPath<<std::endl;
            j++;
        }
        i++;
    }
}


void PrintRequestInfo(const RequestParsser &Request)
{
    std::cout << std::endl<<"++++++++++++++++++| Http  Method |++++++++++++++++"<<std::endl;
    std::cout<< Request.GetHttpMethod()<<std::endl;

    std::cout << std::endl<<"++++++++++++++++++|  Http   URI  |++++++++++++++++"<<std::endl;
    std::cout << Request.GetPath()<<std::endl;

    // std::cout << std::endl<<"++++++++++++++++++| Http Version |++++++++++++++++"<<std::endl;
    // std::cout << Request.GetHttpVersion()<<std::endl;
    std::cout << std::endl<<"++++++++++++++++++| Http Headers |++++++++++++++++"<<std::endl;
    Request.PrintHeaders();
    std::cout << std::endl<<"++++++++++++++++++|  Http Request Body  |++++++++++++++++"<<std::endl;
    if(Request.GetBody().empty())
        std::cout << "No Body in Request :("<<std::endl;
    else
        std::cout << Request.GetBody()<<std::endl;
}

int StringToInt(const std::string &s)
{
    int nbr;
    std::istringstream num(s);

    num >> nbr;
    return nbr;
}

std::string intToString(int value) {
    std::ostringstream os;
    os << value;
    return os.str();
}

#include <cstring>
void SetSocketToNonBlocking(int fd)
{

    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) 
    {
        throw std::runtime_error(std::string("fcntl: ") + strerror(errno));
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) 
    {
        throw std::runtime_error(std::string("fcntl: ") + strerror(errno));
    }
}



void deleteDir(const std::string& path) 
{
    DIR* dir = opendir(path.c_str());

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) 
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) 
        {
            continue;
        }
         std::string fullPath = path + "/" + entry->d_name;
        if (entry->d_type == DT_DIR) 
        {
            deleteDir(fullPath);
            std::remove(fullPath.c_str()); 
        } 
        else 
        {
            if (std::remove(fullPath.c_str()) != 0) 
            {
                std::cerr << "Error removing file: " << fullPath << std::endl;
            }
        }
    }

    closedir(dir);
}

bool isDirectory(const std::string& path)
{
    struct stat fileStat;
    if (stat(path.c_str(), &fileStat) == 0)
        return S_ISDIR(fileStat.st_mode);
    else
        std::cerr << "Error: " << std::strerror(errno) << std::endl;
    return false;
}

std::string removeLast(std::string str) 
{
    if (!str.empty()) 
    {
        str.resize(str.size() - 1);
    }
    return str;
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

std::string GetHttpStatusMessage(HTTPStatusCode StatusCode)
{ 
    switch(StatusCode) 
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


std::string ModifyHtmlPage(std::string &htmlPage, HTTPStatusCode statusCode, std::string Path) 
{
    std::string modifiedPage = htmlPage;
    std::string statusCodeStr;
    std::ostringstream oss;
    oss << statusCode;
    statusCodeStr = oss.str();
    std::string statusMessage = GetHttpStatusMessage(statusCode);

    std::string statusCodePlaceholder = "<span class=\"status-code\">number</span>";
    
    if(Path == "./HtmlPages/notfound.html")
    {
        size_t startPos = modifiedPage.find(statusCodePlaceholder);
        if (startPos != std::string::npos) 
        {
            modifiedPage.replace(startPos, statusCodePlaceholder.length(), "<span class=\"status-code\">" + statusCodeStr + "</span>");
        }

        std::string statusMessagePlaceholder = "<p>string.</p>";
        
        startPos = modifiedPage.find(statusMessagePlaceholder);
        if (startPos != std::string::npos) 
        {
            modifiedPage.replace(startPos, statusMessagePlaceholder.length(), "<p>" + statusMessage + ".</p>");
        }
    }
    return modifiedPage;
}

std::string err_pages(t_servers ServerSetting, int index, HTTPStatusCode StatusCode)
{

    std::string errPage = "./HtmlPages/notfound.html";
    std::string tmp;
    if(index == -2)
    {
        if(ServerSetting.errPage.empty())
        {
            ServerSetting.errPage = errPage;
        }
    }
    else 
    {
        if(ServerSetting.locations[index].errPage.empty())
        {
            if(ServerSetting.errPage.empty())
            {
                ServerSetting.locations[index].errPage = errPage;
            }
            else 
            {
                ServerSetting.locations[index].errPage = ServerSetting.errPage;
            }
        }
    
    }
    if(index == -2)
    {
        tmp = HtmlToSring(ServerSetting.errPage);
        tmp = ModifyHtmlPage(tmp, StatusCode, ServerSetting.errPage);
    }
    else 
    {
        tmp = HtmlToSring(ServerSetting.locations[index].errPage);
        tmp = ModifyHtmlPage(tmp, StatusCode, ServerSetting.locations[index].errPage);
    }
    return tmp;
}
 


std::string Delete (RequestParsser &Request, HttpResponse &Response, t_servers &setting, int index, std::string Uri)
{
    std::string errPages;
    
    if (access(Uri.c_str(), F_OK) == -1)
    {
        Response.SetHTTPStatusCode(HTTP_NOT_FOUND);
        
        errPages = err_pages(setting, index, HTTP_NOT_FOUND);
        return errPages;
    }
    if (isDirectory(Uri)) 
    {         
        if (!Uri.empty() && Request.GetFlage() == true) 
        {
            if (access(Uri.c_str(), W_OK) == 0) 
            {
                deleteDir(Uri); 
                std::remove(Uri.c_str());
                Response.SetHTTPStatusCode(HTTP_NO_CONTENT);
                
                return "";
            }
            else 
            {        
                Response.SetHTTPStatusCode(HTTP_FORBIDDEN);
                errPages = err_pages(setting, index, HTTP_FORBIDDEN);
                return errPages;            
            }
        }
        else 
        {
            Response.SetHTTPStatusCode(HTTP_CONFLICT);
            errPages = err_pages(setting, index, HTTP_CONFLICT); 
            return errPages;
        }
        
    }
    else 
    {
        if (std::remove(Uri.c_str()) != 0)
        {
            Response.SetHTTPStatusCode(HTTP_INTERNAL_SERVER_ERROR);
            errPages = err_pages(setting, index, HTTP_INTERNAL_SERVER_ERROR); 
            return errPages;        
        }
        else
        {
            Response.SetHTTPStatusCode(HTTP_NO_CONTENT);
            errPages = err_pages(setting, index, HTTP_NO_CONTENT); 
            return errPages;        
        }
    }
}




































