//
// Created by oleg on 08.04.18.
//

#ifndef CIFAR10CONVERTER_TYPES_HPP
#define CIFAR10CONVERTER_TYPES_HPP

#include <memory>
#include <sstream>

class Logger
{
public:
	virtual ~Logger() = default;

	virtual void Info(const std::string& message) = 0;
	virtual void Error(const std::string& message) = 0;
};

extern std::unique_ptr<Logger> g_Log;

#define ERROR(msg) do                        \
                   {                         \
                     std::stringstream ss;   \
                     ss << msg;              \
                     g_Log->Error(ss.str()); \
                   } while (0)
#define INFO(msg)  do                       \
                   {                        \
                     std::stringstream ss;  \
                     ss << msg;             \
                     g_Log->Info(ss.str()); \
                   } while (0)

class Size
{
public:
	Size();
	Size(int width, int height);

	int m_Width;
	int m_Height;
};
std::ostream& operator<<(std::ostream& s, const Size& size);

#endif //CIFAR10CONVERTER_TYPES_HPP
