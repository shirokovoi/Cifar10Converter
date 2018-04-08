//
// Created by oleg on 08.04.18.
//

#include <iomanip>
#include <iostream>
#include "DefaultLogger.hpp"

void DefaultLogger::Info(const std::string& message)
{
	PutFormatedTime(std::cout);
	std::cout << ": \033[1;32m" << message << "\033[0m" << std::endl;
}

void DefaultLogger::Error(const std::string& message)
{
	PutFormatedTime(std::cerr);
	std::cerr << ": \033[1;31m" << message << "\033[0m" << std::endl;
}

void DefaultLogger::PutFormatedTime(std::ostream& s)
{
	std::time_t t = std::time(nullptr);
	std::tm tm = *std::localtime(&t);
	s << std::put_time(&tm, "%F %T");
}
