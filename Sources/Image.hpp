//
// Created by oleg on 08.04.18.
//

#ifndef CIFAR10CONVERTER_IMAGE_HPP
#define CIFAR10CONVERTER_IMAGE_HPP

#include <vector>
#include <cstdint>
#include <Types.hpp>

class Image
{
public:
	Image();

	std::shared_ptr<Image> Resize(Size target_size);

	int m_Channels;
	Size m_Size;
	int m_Label;
	std::vector<uint8_t> m_Data;
};


#endif //CIFAR10CONVERTER_IMAGE_HPP
