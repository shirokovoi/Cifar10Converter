//
// Created by oleg on 08.04.18.
//

#ifndef CIFAR10CONVERTER_DEFAULTLOGGER_HPP
#define CIFAR10CONVERTER_DEFAULTLOGGER_HPP

#include <Types.hpp>

class DefaultLogger: public Logger
{
public:
	void Info(const std::string &message) override;
	void Error(const std::string &message) override;

private:
	void PutFormatedTime(std::ostream& s);
};

#endif //CIFAR10CONVERTER_DEFAULTLOGGER_HPP
