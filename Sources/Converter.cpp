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
#include <Cxx.hpp>
#include <DefaultLogger.hpp>

int main(int argc, char** argv)
{
	g_Log = std::make_unique<DefaultLogger>();

	const int batches_number = 5;
	const int images_per_batch = 10000;

	Size target_size = {320, 320};

	std::unique_ptr<caffe::db::DB> train_db(caffe::db::GetDB("lmdb"));
	train_db->Open("./cifar10_train_lmdb", caffe::db::NEW);

	for (int batch_index = 1; batch_index < batches_number + 1; batch_index++)
	{
		std::unique_ptr<caffe::db::Transaction> train_transaction(train_db->NewTransaction());
		if (!train_transaction)
		{
			MERROR("Failed to create new train transaction");
			return -1;
		}
		const std::string batch_path = "./data_batch_" + std::to_string(batch_index) + ".bin";
		auto batch = Batch::FromFile(batch_path);
		if (!batch)
		{
			MERROR("Failed to open batch!");
			return -1;
		}

		for (int image_index = 0; image_index < images_per_batch; image_index++)
		{
			auto image = batch->GetNext();
			if (!image)
			{
				MERROR("Can't get next image #" << image_index << " from batch #" << batch_index);
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
		INFO("Batch #" << batch_index << " processed");
		train_transaction->Commit();
	}

	train_db->Close();

	std::unique_ptr<caffe::db::DB> test_db(caffe::db::GetDB("lmdb"));
	test_db->Open("./cifar10_test_lmdb", caffe::db::NEW);
	std::unique_ptr<caffe::db::Transaction> test_transaction(test_db->NewTransaction());
	if (!test_transaction)
	{
		MERROR("Failed to create new test transaction");
		return -1;
	}

	auto test_batch = Batch::FromFile("./test_batch.bin");
	if (!test_batch)
	{
		MERROR("Failed to open test batch");
		return -1;
	}
	for (int image_index = 0; image_index < images_per_batch; image_index++)
	{
		auto image = test_batch->GetNext();
		if (!image)
		{
			MERROR("Can't get next image #" << image_index << " from test batch");
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
	INFO("Test batch processed");

	test_transaction->Commit();
	test_db->Close();

	return 0;
}
