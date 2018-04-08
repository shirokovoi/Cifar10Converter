//
// Created by oleg on 08.04.18.
//

#ifndef CIFAR10CONVERTER_CXX_HPP
#define CIFAR10CONVERTER_CXX_HPP

namespace std {

template<typename T, typename... Args> std::unique_ptr<T> make_unique(Args&&... args)
{
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}


#endif //CIFAR10CONVERTER_CXX_HPP
