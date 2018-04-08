//
// Created by oleg on 08.04.18.
//

#include <istream>
#include <fstream>
#include <Batch.hpp>
#include <Cxx.hpp>

Batch::Batch(std::unique_ptr<std::istream> stream):
    m_Input(std::move(stream))
{ }

std::unique_ptr<Batch> Batch::FromFile(const std::string &path)
{
	auto file = std::make_unique<std::ifstream>(path);
	if (!file->is_open())
	{
		ERROR("Failed to open \"" << path << "\"");
		return std::unique_ptr<Batch>();
	}

	return std::make_unique<Batch>(std::move(file));
}

std::shared_ptr<Image> Batch::GetNext()
{
	const Size resolution = {32, 32};
	const int channels = 3;

	uint8_t label_byte = 0;
	std::vector<uint8_t> data(channels * resolution.m_Width * resolution.m_Height);

	m_Input->read((char*)&label_byte, 1);
	m_Input->read((char*)data.data(), data.size());

	auto image = std::make_shared<Image>();
	image->m_Label = (int)label_byte;
	image->m_Data = std::move(data);
	image->m_Channels = 3;
	image->m_Size = {32, 32};

	return image;
}
