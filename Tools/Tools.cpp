

#include "Tools.hpp"

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
    while(i < ServerSetting.size())
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
        while(j < ServerSetting[i].locations.size())
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


void PrintRequestInfo(const HttpRequest &Request)
{
    std::cout << std::endl<<"++++++++++++++++++| Http  Method |++++++++++++++++"<<std::endl;
    std::cout<< Request.GetHttpMethod()<<std::endl;

    std::cout << std::endl<<"++++++++++++++++++|  Http   URI  |++++++++++++++++"<<std::endl;
    std::cout << Request.GetPath()<<std::endl;

    std::cout << std::endl<<"++++++++++++++++++| Http Version |++++++++++++++++"<<std::endl;
    std::cout << Request.GetHttpVersion()<<std::endl;
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


