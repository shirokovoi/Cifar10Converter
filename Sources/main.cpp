//
// Created by oleg on 08.04.18.
//

#include <Types.hpp>
#include <string>
#include <iostream>
#include <ctime>
#include <iomanip>

#include <Batch.hpp>

#include <caffe/util/db.hpp>
#include <caffe/util/format.hpp>
#include <caffe/proto/caffe.pb.h>
#include "Cxx.hpp"

class DefaultLogger: public Logger
{
public:
	void Info(const std::string& message) override
	{
		PutFormatedTime(std::cout);
		std::cout << ": \033[1;32m" << message << "\033[0m" << std::endl;
	}

	void Error(const std::string& message) override
	{
		PutFormatedTime(std::cerr);
		std::cerr << ": \033[1;31m" << message << "\033[0m" << std::endl;
	}

private:
	void PutFormatedTime(std::ostream& s)
	{
		std::time_t t = std::time(nullptr);
		std::tm tm = *std::localtime(&t);
		s << std::put_time(&tm, "%F %T");
	}
};

std::unique_ptr<Logger> g_Log;

int main(int argc, char** argv)
{
	g_Log = std::make_unique<Logger>();

	const int batches_number = 5;
	const int images_per_batch = 10000;

	Size target_size = {320, 320};

	std::unique_ptr<caffe::db::DB> train_db(caffe::db::GetDB("lmdb"));
	train_db->Open("./cifar10_train_lmdb", caffe::db::NEW);
	std::unique_ptr<caffe::db::Transaction> train_transaction(train_db->NewTransaction());

	for (int batch_index = 0; batch_index < batches_number; batch_index++)
	{
		const std::string batch_path = "./data_batch_" + std::to_string(batch_index) + ".bin";
		auto batch = Batch::FromFile(batch_path);

		for (int image_index = 0; image_index < images_per_batch; image_index++)
		{
			auto image = batch->GetNext();
			if (!image)
			{
				ERROR("Can't get next image #" << image_index << " from batch #" << batch_index);
				return -1;
			}

			auto resized = image->Resize(target_size);
			caffe::Datum datum;
			datum.set_channels(3);
			datum.set_width(resized->m_Size.m_Width);
			datum.set_height(resized->m_Size.m_Height);
			datum.set_label(resized->m_Label);
			datum.set_data(resized->m_Data.data(), resized->m_Data.size());

			std::string serialized;
			datum.SerializeToString(&serialized);
			int key = batch_index * images_per_batch + image_index;
			train_transaction->Put(std::to_string(key), serialized);
		}
	}

	train_transaction->Commit();
	train_db->Close();

	std::unique_ptr<caffe::db::DB> test_db(caffe::db::GetDB("lmdb"));
	test_db->Open("./cifar10_test_lmdb", caffe::db::NEW);
	std::unique_ptr<caffe::db::Transaction> test_transaction(train_db->NewTransaction());
	auto test_batch = Batch::FromFile("./test_batch.bin");
	for (int image_index = 0; image_index < images_per_batch; image_index++)
	{
		auto image = test_batch->GetNext();
		if (!image)
		{
			ERROR("Can't get next image #" << image_index << " from test batch");
			return -1;
		}

		auto resized = image->Resize(target_size);
		caffe::Datum datum;
		datum.set_channels(3);
		datum.set_width(resized->m_Size.m_Width);
		datum.set_height(resized->m_Size.m_Height);
		datum.set_label(resized->m_Label);
		std::string serialized;
		datum.SerializeToString(&serialized);
		test_transaction->Put(std::to_string(image_index), serialized);
	}

	test_transaction->Commit();
	test_db->Close();
}