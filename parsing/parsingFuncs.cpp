#include "Parsing.hpp"

bool Parsing::isWhiteSpace(char c){	
	if (c == ' ' || c == '\t'  || c == '\n')
		return false;
	return true;
}

size_t Parsing::countAppearance(std::string str, char c){
	size_t counter = 0;
	size_t idx = 0;
	while (str[idx]){
		if (str[idx] == c)
			counter++;
		idx++;
	}
	return counter;
}

bool Parsing::ft_strchr(std::string str, char c){
	int idx = 0;
	while (str[idx]){
		if (str[idx] == c)
			return true;
		idx++;
	}
	return false;
}

size_t Parsing::wordsCounter(std::string str, std::string sp){
	size_t counter = 0;
	size_t idx = 0;
	bool trigger = true;

	while (str[idx]){
		if (trigger && !ft_strchr(sp, str[idx])){
			counter++;
			trigger = false;
		}
		if (ft_strchr(sp, str[idx]))
			trigger = true;
		idx++;
	}
	return counter;
}

std::vector<std::string> Parsing::split(std::string str, std::string delimiter){
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

std::vector<std::string> Parsing::ft_split(std::string str, std::string c){
	size_t wordsCount = wordsCounter(str, c);
	size_t splitedIdx = 0;
	size_t strIdx = 0;
	std::vector<std::string> splited;
	if (wordsCount <= 0)
		return splited;
	
	while (true){
		if (str.find(c) == std::string::npos)
			break;
		splited.push_back(str.substr(0, str.find(c) + 1));
		str.erase(0, str.find(c) + 1);
	}
	if (str.length() > 0)
		splited.push_back(str);		
	return splited;
}

std::string Parsing::removeBegeningWhiteSpace(void){
	size_t idx = 0;
	size_t len = this->block.length();
	std::string newStr = "";
	while (idx < len && (this->block[idx] == ' ' || this->block[idx] == '\n' || this->block[idx] == '\t'))
		idx++;
	while (idx < len)
		newStr += this->block[idx++];
	return newStr;
}

bool Parsing::checkBlockCurlyBraces(void) const{
	unsigned int openCurlBraceCount = 0,
	closeCurlBraceCount = 0;
	size_t blockLength = 0;
	blockLength = this->block.length();
	for (int idx = 0; idx < blockLength; idx++){
		if (this->block[idx] == '{')
			openCurlBraceCount++;
		if (this->block[idx] == '}')
			closeCurlBraceCount++;
	}

	if (openCurlBraceCount != closeCurlBraceCount)
		return false;
	return true;
}

std::string Parsing::getBlockName(void){
	size_t idx = 0;
	size_t len = this->block.length();
	while (idx < len){
		if (this->block[idx] == ';' || this->block[idx] == '{')
			break;
		idx++;
	}
	if (idx == 0)
		return this->block;
	return ft_trim(this->block.substr(0, idx));
}

size_t Parsing::countServers(void) {
	size_t counter = 0;
	std::string server = "server";
	while (this->isEndFile() == false){
		readBlock();
		if (this->block.length() == 0)
			break;
		if (getBlockName().compare("server") == 0)
			counter++;
		// std::cout << getBlockName() << std::endl;
	}
	this->reset();
	return counter;
}


std::string Parsing::ft_trim(std::string block){
	size_t len = block.length();
	std::string newStr = "";
	size_t idx = 0;

	// len = block.find_first_not_of(" \t\n");
	if (len == 0)
		return newStr;
	len--;
	while (len > 0 && (block[len] == ' ' || block[len] == '\t' || block[len] == '\n'))
		len--;
	while (block[idx] && (block[idx] == ' ' || block[idx] == '\t' || block[idx] == '\n'))
		idx++;
	while (idx <= len)
		newStr += block[idx++];
	return newStr;
}

void  Parsing::trimVector(std::vector<std::string> &data){
	std::vector<std::string>::iterator it;

	it = data.begin();
	while (it != data.end()){
		*it = ft_trim(*it);
		*it++;
	}
}


size_t Parsing::getOpenCurlyBracePos(std::vector<std::string> data){
	size_t idx = 0;
	std::vector<std::string>::iterator it;

	it = data.begin();
	while (it != data.end()){
		if (it->find("{") != std::string::npos)
			return idx;
		idx++;
		*it++;
	}
	return  -1;
}

void printVec(std::vector<std::string> str){
	std::vector<std::string>::iterator itr;

	itr = str.begin();
	while (itr != str.end()){
		std::cout << *itr << std::endl;
		std::cout << "|||||||||||||||" << std::endl;
		*itr++;
	}
}


int Parsing::findWordPos(std::string str, std::string word){
	size_t idx;
	size_t idx2;

	idx = 0;
	while (str[idx]){
		idx2 = 0;
		while (str[idx + idx2] == word[idx2])
			idx2++;
		if (idx2 == word.length())
			return idx + idx2;
		idx++;
	}
	return -1;
}


std::string Parsing::replaceChar(std::string str, std::string toReplace, char with){
	size_t idx = 0;
	size_t idx2;
	bool trigger = false;
	std::string newStr = "";
	while (str[idx]){
		idx2 = 0;
		while (toReplace[idx2]){
			if (str[idx] == toReplace[idx2]){
				trigger = true;
				break;
			}
			idx2++;
		}
		if (trigger){
			newStr += with;
			trigger = !trigger;
		}else {
			newStr += str[idx];
		}
		idx++;

	}
	return newStr;
}

std::string Parsing::removeChars(std::string str, std::string toRemove){
	size_t idx = 0;
	std::string newStr = "";

	while (str[idx]){
		if (toRemove.find_first_of(str[idx]) == std::string::npos){
			newStr += str[idx];			
		}
		idx++;
	}
	return newStr;
}