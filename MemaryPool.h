#ifndef HEADER_MMEMARY
#define HEADER_MMEMARY

#include <new>
#include <functional>
#include <thread>
#include <mutex>
#include <vector>

static const int __align = 8;
static const int __max_bytes = 128;
static const int __number_of_free_lists = __max_bytes / __align;
static const int __number_add_nodes = 20;

class CMemaryPool
{
public:
	CMemaryPool();
	//bulk memory size. everytime add nodes num
	CMemaryPool(const int large_sz, const int add_num);
	~CMemaryPool();

	//for object. invocation of constructors and destructors
	template<typename T, typename... Args >
	T* PoolNew(Args&&... args);
	template<typename T>
	void PoolDelete(T* &c);

	//for continuous memory
	template<typename T>
	T* PoolMalloc(size_t size);
	template<typename T>
	void PoolFree(T* &m, size_t len);

	//for bulk memory
	template<typename T>
	T* PoolLargeMalloc();
	template<typename T>
	void PoolLargeFree(T* &m);

	std::thread::id GetCreateThreadId();

private:
	size_t RoundUp(size_t size) {
		return ((size + __align - 1) & ~(__align - 1));
	}

	size_t FreeListIndex(size_t size) {
		return (size + __align - 1) / __align - 1;
	}

	void* ReFill(int size, int num = __number_add_nodes, bool is_large = false);
	void* ChunkAlloc(int size, int& nums, bool is_large = false);
	
private:
	union MemNode {
		MemNode*	_next;
		char		_data[1];
	};

	MemNode*	_free_list[__number_of_free_lists];	
	std::mutex	_mutex;
	char*		_pool_start;					
	char*		_pool_end;				
	std::thread::id		_create_thread_id;
	std::vector<char*>  _malloc_vec;

	MemNode*	_free_large;						//bulk memory list
	int			_number_large_add_nodes;			//everytime add nodes num
	int			_large_size;						//bulk memory size
};

template<typename T, typename... Args>
T* CMemaryPool::PoolNew(Args&&... args)
{
	int sz = sizeof(T);
	if (sz > __max_bytes) {
		void* bytes = malloc(sz);
		T* res = new(bytes) T(std::forward<Args>(args)...);
		return res;
	}

	std::unique_lock<std::mutex> lock(_mutex);
	MemNode** my_free = &(_free_list[FreeListIndex(sz)]);
	MemNode* result = *my_free;
	if (result == nullptr) {
		void* bytes = ReFill(RoundUp(sz));
		T* res = new(bytes) T(std::forward<Args>(args)...);
		return res;
	}

	*my_free = result->_next;
	T* res = new(result) T(std::forward<Args>(args)...);
	return res;
}

template<typename T>
void CMemaryPool::PoolDelete(T* &c) {
	if (!c){
		return;
	}

	int sz = sizeof(T);
	if (sz > __max_bytes) {
		free(c);
		return;
	}

	MemNode* node = (MemNode*)c;
	MemNode** my_free = &(_free_list[FreeListIndex(sz)]);

	std::unique_lock<std::mutex> lock(_mutex);
	c->~T();
	node->_next = *my_free;
	*my_free = node;
	c = nullptr;
}

template<typename T>
T* CMemaryPool::PoolMalloc(size_t sz) {
	if (sz > __max_bytes) {
		void* bytes = malloc(sz);
		memset(bytes, 0, sz);
		return (T*)bytes;
	}

	std::unique_lock<std::mutex> lock(_mutex);
	MemNode** my_free = &(_free_list[FreeListIndex(sz)]);
	MemNode* result = *my_free;
	if (result == nullptr) {
		void* bytes = ReFill(RoundUp(sz));
		memset(bytes, 0, sz);
		return (T*)bytes;
	}

	*my_free = result->_next;
	memset(result, 0, sz);
	return (T*)result;
}

template<typename T>
void CMemaryPool::PoolFree(T* &m, size_t len) {
	if (!m) {
		return;
	}

	if (len > __max_bytes) {
		free(m);
		m = nullptr;
		return;
	}

	MemNode* node = (MemNode*)m;
	MemNode** my_free = &(_free_list[FreeListIndex(len)]);

	std::unique_lock<std::mutex> lock(_mutex);
	node->_next = *my_free;
	*my_free = node;
	m = nullptr;
}

template<typename T>
T* CMemaryPool::PoolLargeMalloc() {
	if (_number_large_add_nodes == 0 || _large_size == 0) {
		throw std::exception("Large block of memory is not set!");
		return nullptr;
	}
	std::unique_lock<std::mutex> lock(_mutex);
	MemNode** my_free = &_free_large;
	MemNode* result = _free_large;
	if (result == nullptr) {
		void* bytes = ReFill(RoundUp(_large_size), _number_large_add_nodes, true);
		memset(bytes, 0, _large_size);
		return (T*)bytes;
	}

	*my_free = result->_next;
	memset(result, 0, _large_size);
	return (T*)result;
}

template<typename T>
void CMemaryPool::PoolLargeFree(T* &m) {
	if (!m) {
		return;
	}
	MemNode* node = (MemNode*)m;
	MemNode** my_free = &_free_large;

	std::unique_lock<std::mutex> lock(_mutex);
	node->_next = *my_free;
	*my_free = node;
	m = nullptr;
}

#endif