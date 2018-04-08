//
// Created by oleg on 08.04.18.
//

#include <Batch.hpp>
#include <DefaultLogger.hpp>
#include <Cxx.hpp>

#include <caffe/util/db.hpp>
#include <caffe/util/format.hpp>
#include <caffe/proto/caffe.pb.h>

int main(int argc, char** argv)
{
	g_Log = std::make_unique<DefaultLogger>();

	std::unique_ptr<caffe::db::DB> train_db(caffe::db::GetDB("lmdb"));
	train_db->Open("./cifar10_train_lmdb", caffe::db::READ);

	std::set<std::string> keys;
	std::set<size_t> sizes;

	std::unique_ptr<caffe::db::Cursor> cursor(train_db->NewCursor());
	while (cursor->valid())
	{
		keys.emplace(cursor->key());
		sizes.emplace(cursor->value().size());

		cursor->Next();
	}

	train_db->Close();

	std::unique_ptr<caffe::db::DB> test_db(caffe::db::GetDB("lmdb"));
	test_db->Open("./cifar10_test_lmdb", caffe::db::READ);

	std::set<std::string> test_keys;
	std::set<std::string> test_sizes;

	cursor = std::unique_ptr<caffe::db::Cursor>(test_db->NewCursor());
	while (cursor->valid())
	{
		test_keys.emplace(cursor->key());
		test_sizes.emplace(cursor->value().size());

		cursor->Next();
	}

	train_db->Close();

	std::stringstream ss;
	bool is_first = true;
	for (auto& s: sizes)
	{
		if (!is_first)
			ss << " ";
		is_first = false;
		ss << s;
	}

	std::stringstream test_ss;
	is_first = true;
	for (auto& s: test_sizes)
	{
		if (!is_first)
			ss << " ";
		is_first = false;
		ss << s;
	}
	INFO("Train set contains " << keys.size() << " records and " << sizes.size() << " values with different size (" << ss.str() << ")");
	INFO("Test set contains " << test_keys.size() << " record and " << test_sizes.size() << " values with different size (" << test_ss.str() << ")");
	return 0;
}