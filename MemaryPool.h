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
	void* PoolMalloc(size_t size);
	void PoolFree(void* m, size_t len);

	//for bulk memory
	void* PoolLargeMalloc();
	void PoolLargeFree(void* m);

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
void CMemaryPool::PoolDelete(T* &c)
{
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

#endif