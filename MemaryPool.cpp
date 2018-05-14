#include "MemaryPool.h"
#include <assert.h>

CMemaryPool::CMemaryPool() {
	memset(_free_list, 0, sizeof(_free_list[__number_of_free_lists]));
	_create_thread_id = std::this_thread::get_id();
}

CMemaryPool::CMemaryPool(const int large_sz, const int add_num) : _large_size(large_sz), _number_large_add_nodes(add_num){
	memset(_free_list, 0, sizeof(_free_list[__number_of_free_lists]));
	_create_thread_id = std::this_thread::get_id();
}

CMemaryPool::~CMemaryPool() {
	assert(_create_thread_id == std::this_thread::get_id());
	for (auto iter = _malloc_vec.begin(); iter != _malloc_vec.end(); ++iter) {
		if (*iter) {
			free(*iter);
		}
	}
}

std::thread::id CMemaryPool::GetCreateThreadId() {
	return _create_thread_id;
}

void* CMemaryPool::ReFill(int size, int num, bool is_large) {
	int nums = num;

	char* chunk = (char*)ChunkAlloc(size, nums);
	MemNode** my_free;
	MemNode* res, *current, *next;
	if (1 == nums) {
		return chunk;
	}

	res = (MemNode*)chunk;

	if (is_large) {
		my_free = &_free_large;

		*my_free = next = (MemNode*)(chunk + size);
		for (int i = 1;; i++) {
			current = next;
			next = (MemNode*)((char*)next + size);
			if (nums - 1 == i) {
				current->_next = nullptr;
				break;

			}
			else {
				current->_next = next;
			}
		}

	} else {
		my_free = &(_free_list[FreeListIndex(size)]);

		*my_free = next = (MemNode*)(chunk + size);
		for (int i = 1;; i++) {
			current = next;
			next = (MemNode*)((char*)next + size);
			if (nums - 1 == i) {
				current->_next = nullptr;
				break;

			}
			else {
				current->_next = next;
			}
		}
	}
	return res;
}

void* CMemaryPool::ChunkAlloc(int size, int& nums, bool is_large) {
	char* res;
	size_t need_bytes = size * nums;
	size_t left_bytes = _pool_end - _pool_start;

	//内存池够用
	if (left_bytes >= need_bytes) {
		res = _pool_start;
		_pool_start += need_bytes;
		return res;
	
	} else if (left_bytes >= size) {
		nums = left_bytes / size;
		need_bytes = size * nums;
		res = _pool_start;
		_pool_start += need_bytes;
		return res;

	} 
	size_t bytes_to_get = size * nums;

	if (!is_large) {
		if (left_bytes > 0) {
			MemNode* my_free = _free_list[FreeListIndex(left_bytes)];
			((MemNode*)_pool_start)->_next = my_free;
			_free_list[FreeListIndex(size)] = (MemNode*)_pool_start;
		}

	} else {
		free(_pool_start);
	}
	

	_pool_start = (char*)malloc(bytes_to_get);
	
	//内存分配失败
	if (0 == _pool_start) {
		throw std::exception("There memary is not enough!");
	}

	_malloc_vec.push_back(_pool_start);
	_pool_end = _pool_start + bytes_to_get;
	return ChunkAlloc(size, nums, is_large);
}