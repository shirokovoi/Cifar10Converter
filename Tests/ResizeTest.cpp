//
// Created by oleg on 08.04.18.
//

#include <gtest/gtest.h>
#include <Image.hpp>
#include <fstream>

std::shared_ptr<Image> ReadImage()
{
	std::ifstream input("./input.ppm");
	std::string magic;
	input >> magic;
	if (magic != "P3")
	{
		return std::shared_ptr<Image>();
	}

	Size resolution;
	input >> resolution.m_Width;
	input >> resolution.m_Height;

	auto image = std::make_shared<Image>();
	image->m_Label = 1;
	image->m_Size = resolution;
	image->m_Channels = 3;
	image->m_Data.resize(image->m_Channels * image->m_Size.m_Width * image->m_Size.m_Height);

	for (size_t i = 0; i < image->m_Data.size(); i++)
	{
		int value = 0;
		input >> value;
		image->m_Data[i] = (uint8_t)value;
	}

	return image;
}

void StoreImage(const std::string path, std::shared_ptr<Image> image)
{
	std::ofstream output(path);
	if (!output.is_open())
	{
		return;
	}

	output << "P3" << std::endl;
	output << image->m_Size.m_Width << ' ' << image->m_Size.m_Height << std::endl;

	bool is_first = true;
	for (auto& byte: image->m_Data)
	{
		if (!is_first)
		{
			output << " ";
		}
		is_first = false;
		output << (int)byte;
	}
}

TEST(ResizeTest, LabelRestore)
{
	auto image = ReadImage();
	ASSERT_TRUE((bool)image);
	int origin_label = image->m_Label;
	auto scaled = image->Resize({64,64});
	ASSERT_TRUE((bool)scaled);
	int scaled_label = scaled->m_Label;

	ASSERT_EQ(origin_label, scaled_label);
	SUCCEED();
}

TEST(ResizeTest, ReaderTest)
{
	auto image = ReadImage();
	ASSERT_TRUE((bool)image);
	StoreImage("./readen.ppm", image);
	SUCCEED();
}

TEST(ResizeTest, Resize)
{
	auto image = ReadImage();
	ASSERT_TRUE((bool)image);
	auto scaled = image->Resize({64, 64});
	ASSERT_TRUE((bool)scaled);
	StoreImage("./scaled.ppm", scaled);
	SUCCEED();
}
