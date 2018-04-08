//
// Created by oleg on 08.04.18.
//

#include <Image.hpp>
#include <opencv2/opencv.hpp>

Image::Image():
    m_Label(-1)
{}

std::shared_ptr<Image> Image::Resize(Size target_size)
{
	auto resized = std::make_shared<Image>();
	resized->m_Channels = m_Channels;
	resized->m_Label = m_Label;
	resized->m_Size = target_size;
	resized->m_Data.resize(resized->m_Channels * resized->m_Size.m_Width * resized->m_Size.m_Height);

	cv::Mat cv_input_image(m_Size.m_Width, m_Size.m_Height, CV_8UC3, m_Data.data());
	cv::Mat cv_output_image(target_size.m_Width, target_size.m_Height, CV_8UC3, resized->m_Data.data());
	cv::Size cv_target_size(target_size.m_Width, target_size.m_Height);
	cv::resize(cv_input_image, cv_output_image, cv_target_size, 0, 0);

	return resized;
}

