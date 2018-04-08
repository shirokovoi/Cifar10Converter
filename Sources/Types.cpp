//
// Created by oleg on 08.04.18.
//

#include <Types.hpp>

Size::Size():
    m_Width(0),
    m_Height(0)
{ }

Size::Size(int width, int height):
    m_Width(width),
    m_Height(height)
{ }

std::ostream& operator<<(std::ostream& s, const Size& size)
{
	s << size.m_Width << 'x' << size.m_Height;
	return s;
}

