#include "Errors.hpp"

ErrorsChecker::ErrorsChecker(void){
}

ErrorsChecker::ErrorsChecker(std::string path) : conFilePath(path){

}

void ErrorsChecker::checkFile (void) {
	if (!checkExtention(this->conFilePath))
		throw std::runtime_error("\x1b[31mError: The provided file extention is not accepted\nfile.conf is the only extention accepted.");
    this->file.open(this->conFilePath.c_str());
	if (!this->file.is_open()) {
		throw std::runtime_error("\x1b[31mError: Couldn't open or find the provided configuration file\nCheck if path exists or if the file have the reading permission.");
	}
	std::cout << GREEN << "Using " << this->conFilePath << " as a configuration file." << DEFAULT_COLOR << std::endl;
}


// fucntion 
const std::string ErrorsChecker::GetConfigFilePath() const
{
    return this->conFilePath;
}


ErrorsChecker::~ErrorsChecker(void){
	this->file.close();
}

void ErrorsChecker::setConfile(std::string path) {
	this->conFilePath = path;
}

bool ErrorsChecker::checkExtention(std::string path){
	std::string requiredExtention = ".conf";
	size_t dotPos = path.rfind(".");
	if (dotPos == std::string::npos)
		return false;
	if (path.compare(path.rfind("."), 6, requiredExtention) != 0)
		return false;
	return true;
}

std::vector<std::string> ErrorsChecker::split(std::string str, std::string delimiter){
	std::vector<std::string> splited;
	std::string holder;
	size_t idx = 0;
	size_t idx2 = 0;
	while (true){
		idx2 = 0;
		idx = str.find_first_of(delimiter);
		if (idx == std::string::npos)
			break;
		splited.push_back(str.substr(0, idx));
		str = str.erase(0, idx + 1);
	}
	if(str.substr(0, str.length()).length() > 0)
		splited.push_back(str.substr(0, str.length()));
	return splited;

}

void ErrorsChecker::checkUnknownKey(std::vector<t_data> data){
	std::vector<std::string>::iterator itr;
	std::vector<t_data>::iterator dataItr;
	std::vector<std::string> names;
	names.push_back("listen");
	names.push_back("server_names");
	names.push_back("host");
	names.push_back("root");
	names.push_back("proxy_pass");
	names.push_back("allowed_methods");
	names.push_back("index");
	names.push_back("maxBodySize");
	names.push_back("upload_path");
	names.push_back("autoIndex");
	names.push_back("cgi_extentions");
	names.push_back("upload_path");
	names.push_back("cgi_path");
	dataItr = data.begin();
	while (dataItr != data.end()){
		itr = dataItr->rulesNames.begin();
		while (itr != dataItr->rulesNames.end()){
			if (std::find(names.begin(), names.end(), *itr) == names.end())
			{
				std::string error = "\x1b[31mError: Unknown rule " + *itr;
				throw std::runtime_error(error.c_str());
			}
			*itr++;
		}
		*dataItr++;
	}
}
