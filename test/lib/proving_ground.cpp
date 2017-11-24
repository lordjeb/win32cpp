#include "pch.h"
#include "win32api_mocks.h"
#include "handle.h"
#include "win32api_abstractions.h"
#include "map_view_deleter.h"

using namespace std;
using namespace win32cpp;
using testing::Return;
using testing::Eq;
using testing::_;

void bigFunction(shared_ptr<IFile> pFile)
{
	auto file_handle = unique_file_handle{ handle_closer<invalid_handle_traits>{pFile}, pFile->CreateFileW(LR"(C:\test.txt)", GENERIC_ALL, 0, nullptr, CREATE_ALWAYS, FILE_FLAG_DELETE_ON_CLOSE, nullptr) };
	if (file_handle)
	{
		// do something with x
		auto mapping_handle = unique_mapping_handle{ handle_closer<invalid_handle_traits>{pFile}, pFile->CreateFileMappingW(file_handle.get(), nullptr, PAGE_READONLY, 0, 1, nullptr) };
		if (mapping_handle)
		{
			auto view = unique_ptr<BYTE, map_view_deleter>{ (BYTE*)pFile->MapViewOfFile(mapping_handle.get(), FILE_MAP_ALL_ACCESS, 0, 0, 1), map_view_deleter{ pFile } };
			if (view)
			{
				*(view.get()) = 0x00;
			}
		}
	}
}

struct stack_object_deleter
{
	void operator () (void*) const {}
};

const auto fake_handle_value = (HANDLE) 0xBAADF000LL;
const auto fake_handle_value_2 = (HANDLE) 0xBAADF004LL;

TEST(bigFunction_test, fails_to_open_file)
{
	mock_file mocky;
	EXPECT_CALL(mocky, CreateFileW(_, _, _, _, _, _, _))
		.Times(1)
		.WillRepeatedly(Return(INVALID_HANDLE_VALUE));

	bigFunction(shared_ptr<IFile>{ &mocky, stack_object_deleter{} });
}

TEST(bigFunction_test, successfully_opens_file)
{
	mock_file mocky;
	BYTE buffer[1] = { 0 };
	EXPECT_CALL(mocky, CreateFileW(_, _, _, _, _, _, _))
		.Times(1).WillRepeatedly(Return(fake_handle_value));
	EXPECT_CALL(mocky, CreateFileMappingW(fake_handle_value, _, _, _, _, _))
		.Times(1).WillRepeatedly(Return(fake_handle_value_2));
	EXPECT_CALL(mocky, MapViewOfFile(fake_handle_value_2, _, _, _, _))
		.Times(1).WillOnce(Return(&buffer[0]));
	EXPECT_CALL(mocky, UnmapViewOfFile(_))
		.Times(1).WillOnce(Return(TRUE));
	EXPECT_CALL(mocky, CloseHandle(_))
		.Times(2).WillRepeatedly(Return(TRUE));

	bigFunction(shared_ptr<IFile>{ &mocky, stack_object_deleter{} });

	ASSERT_THAT(buffer[0], Eq(0x00));
}
