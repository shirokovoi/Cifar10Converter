//
// Created by oleg on 08.04.18.
//

#ifndef CIFAR10CONVERTER_BATCH_HPP
#define CIFAR10CONVERTER_BATCH_HPP

#include <Image.hpp>
#include <memory>
#include <istream>

class Batch
{
public:
	Batch(std::unique_ptr<std::istream> stream);

	static std::unique_ptr<Batch> FromFile(const std::string& path);
	std::shared_ptr<Image> GetNext();
private:

	std::unique_ptr<std::istream> m_Input;
};

#endif //CIFAR10CONVERTER_BATCH_HPP
