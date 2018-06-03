#pragma once
#include <atomic>
#include <functional>

//reference count class
class CRefCount {
public:
	// construct
	CRefCount() : _uses(1), _weaks(0) {}

	// ensure that derived classes can be destroyed properly
	virtual ~CRefCount() noexcept {}

	// increment use count
	void IncrefUse() {	
		_uses++;
	}

	// increment weak reference count
	void IncrefWeak() {
		_weaks++;
	}

	//decrement use count
	bool DecrefUse() {	
		if (--_uses == 0) {	
			return true;
		}
		return false;
	}

	// decrement weak reference count
	bool DecrefWeak() {
		if (--_weaks == 0) {
			return true;
		}
		return false;
	}

	// return use count
	long GetUseCount() const {	
		return _uses;
	}

	// return true if _uses == 0
	bool Expired() const  {	
		return (_uses == 0);
	}

private:
	std::atomic_long _uses;
	std::atomic_long _weaks;
};

// base class for CMemSharePtr and CMemWeakPtr
template<typename T>
class CBasePtr {
public:
	typedef CBasePtr<T>					_BasePtr;
	typedef std::function<void(T*&)>	_PtrDeleter;
	typedef std::function<void(CRefCount*&)>	_RefDeleter;
	typedef std::pair<_PtrDeleter, _RefDeleter> _Deleter;

	// construct
	CBasePtr() noexcept : _ptr(0), _ref_count(0), _deleter(std::make_pair(nullptr, nullptr)){}
	CBasePtr(T* ptr, CRefCount* ref) noexcept : _ptr(ptr), _ref_count(ref), _deleter(std::make_pair(nullptr, nullptr)) {}
	CBasePtr(T* ptr, CRefCount* ref, _Deleter& func) noexcept : _ptr(ptr), _ref_count(ref), _deleter(func) {}


	// construct CBasePtr object that takes resource from _Right
	CBasePtr(const _BasePtr& r) : _ptr(r._ptr), _ref_count(r._ref_count), _deleter(r._deleter) {
		if (_ref_count) {
			_ref_count->IncrefUse();
		}
	}

	// construct CBasePtr object that takes resource from _Right
	CBasePtr(_BasePtr&& r) : _ptr(r._ptr), _ref_count(r._ref_count), _deleter(r._deleter) {
		r._ptr			= nullptr;
		r._ref_count	= nullptr;
		r._deleter		= std::make_pair(nullptr, nullptr);
	}

	// construct CBasePtr object that takes resource from _Right
	_BasePtr& operator=(_BasePtr&& r) {
		_ptr			= r._ptr;
		_ref_count		= r._ref_count;
		_deleter		= r._deleter;

		r._ptr		    = nullptr;
		r._ref_count    = nullptr;
		r._deleter		= std::make_pair(nullptr, nullptr);
		return (*this);
	}

	// construct CBasePtr object that takes resource from _Right
	_BasePtr& operator=(const _BasePtr& r) {
		_ptr		 = r._ptr;
		_ref_count   = r._ref_count;
		_deleter	 = r._deleter;
		if (_ref_count) {
			_ref_count->IncrefUse();
		}
		return (*this);
	}

	// return use count
	long UseCount() const noexcept {	
		return (_ref_count ? _ref_count->GetUseCount() : 0);
	}

	// return pointer to resource
	T* Get() const noexcept {	
		return (_ptr);
	}

	// test if expired
	bool Expired() const noexcept {	
		return (!_ref_count || _ref_count->Expired());
	}

	// release resource
	void Reset() {	
		Reset(0, 0);
	}
 
	// release resource and take ownership from CMemWeakPtr _Other._Ptr
	void Reset(const _BasePtr& other) {
		Reset(other._ptr, other._ref_count, other._deleter);
	}

	// release resource and take _Other_ptr through _Other_rep
	void Reset(T *other_ptr, CRefCount * other_rep, _Deleter& deleter) {	
		if (other_rep)
			other_rep->IncrefUse();
		_Reset0(other_ptr, other_rep, deleter);
	}

	// release weak reference to resource
	void Resetw() {
		_Resetw(0, 0);
	}

	// release weak reference to resource and take _Other._Ptr
	void Resetw(_BasePtr& other) {
		Resetw(other._ptr, other._ref_count, other._deleter);
	}

	void Resetw(T *other_ptr, CRefCount *other_rep, _Deleter& func) {
		if (other_rep)
			other_rep->IncrefWeak();
		_Resetw0(other_ptr, other_rep, func);
	}

protected:
	// release resource and take _Other_ptr through _Other_rep
	void Reset(T *other_ptr, CRefCount * other_rep) {
		if (other_rep)
			other_rep->IncrefUse();
		_Reset0(other_ptr, other_rep);
	}

	// release resource and take new resource
	void _Reset0(T *other_ptr, CRefCount *other_rep) {
		_DecrefUse();

		_ref_count = other_rep;
		_ptr	   = other_ptr;
		_deleter   = std::make_pair(nullptr, nullptr);
	}

	// release resource and take new resource
	void _Reset0(T *other_ptr, CRefCount *other_rep, _Deleter& func) {
		_DecrefUse();
			
		_ref_count	= other_rep;
		_ptr		= other_ptr;
		_deleter	= std::make_pair(nullptr, nullptr);
	}

	// decrement use reference count
	void _DecrefUse() {
		if (_ref_count && _ref_count->DecrefUse()) {
			_Destroy();
		}
	}

	// decrement use reference count
	void _DecrefWeak() {
		if (_ref_count && _ref_count->DecrefWeak()) {
			_DestroyThis();
		}
	}

	// point to _Other_ptr through _Other_rep
	void _Resetw(T *other_ptr, CRefCount *other_rep) {	
		if (other_rep)
			other_rep->IncrefWeak();
		_Resetw0(other_ptr, other_rep);
	}

	// release resource and take new resource
	void _Resetw0(T *other_ptr, CRefCount *other_rep) {
		_DecrefWeak();

		_ref_count	= other_rep;
		_ptr		= other_ptr;
		_deleter	= std::make_pair(nullptr, nullptr);
	}

	// release resource and take new resource
	void _Resetw0(T *other_ptr, CRefCount *other_rep, _Deleter& func) {
		_DecrefWeak();

		_ref_count	= other_rep;
		_ptr		= other_ptr;
		_deleter	= func;
	}

	//release resource
	virtual void _Destroy() noexcept {
		if (_deleter.first) {
			_deleter.first(_ptr);
		}
		if (_deleter.second) {
			_deleter.second(_ref_count);
		}
	}

	virtual void _DestroyThis() noexcept {
		
	}

	virtual ~CBasePtr() {}

protected:
	T			*_ptr;
	CRefCount	*_ref_count;
	std::pair<_PtrDeleter, _RefDeleter> _deleter;
};

// class for reference counted resource management
template<class T>
class CMemSharePtr : public CBasePtr<T> {	
public:
	// construct
	CMemSharePtr() noexcept : CBasePtr() {}
	CMemSharePtr(T* ptr, CRefCount* ref) noexcept : CBasePtr(ptr, ref) {}
	CMemSharePtr(T* ptr, CRefCount* ref, _Deleter& func) noexcept : CBasePtr(ptr, ref, func) {}

	CMemSharePtr(const _BasePtr& r) : CBasePtr(r) {}
	CMemSharePtr(_BasePtr&& r) : CBasePtr(r) {}

	CMemSharePtr& operator=(_BasePtr&& r) {
		_BasePtr::operator=(r);
		return (*this);
	}

	CMemSharePtr& operator=(const _BasePtr& r) {
		_BasePtr::operator=(r);
		return (*this);
	}

	~CMemSharePtr() {	
		this->_DecrefUse();
	}

	_BasePtr& operator==(const _BasePtr& r) noexcept {
		return _ptr == r._ptr;
	}

	// return pointer to resource
	T *operator->() const noexcept {
		return (this->Get());
	}

	template<typename T2>
	T2 *operator->() const noexcept {
		return dynamic_cast<T2*>(this->Get());
	}

	// return pointer to resource
	T operator*() const noexcept {
		return (*(this->Get()));
	}

	// return true if no other CMemSharePtr object owns this resource
	bool unique() const noexcept {
		return (this->UseCount() == 1);
	}

	// test if CMemSharePtr object owns no resource
	explicit operator bool() const noexcept {
		return (this->Get() != 0);
	}
};

// class for pointer to reference counted resource.
// construc from CMemSharePtr
template<class T>
class CMemWeakPtr : public CBasePtr<T> {
public:
	CMemWeakPtr() {
		Resetw();
	}

	CMemWeakPtr(_BasePtr& r) {
		Resetw(r);
	}

	// construct CBasePtr object that takes resource from _Right
	CMemWeakPtr& operator=(_BasePtr&& r) {
		_BasePtr::operator=(r);
		return (*this);
	}

	// construct CBasePtr object that takes resource from _Right
	CMemWeakPtr& operator=(_BasePtr& r) {
		Resetw(r);
		return (*this);
	}

	// release resource
	~CMemWeakPtr() noexcept {
		this->_DecrefWeak();
	}

	// convert to CMemSharePtr
	CMemSharePtr<T> Lock() const noexcept {
		if (Expired()) {
			return CMemWeakPtr();
		}
		return (CMemSharePtr<T>(*this));
	}
};

template<typename T, typename... Args >
CMemSharePtr<T> MakeNewSharedPtr(CMemaryPool& pool, Args&&... args) {
	T* o = pool.PoolNew<T>(std::forward<Args>(args)...);
	CRefCount* ref = pool.PoolNew<CRefCount>();
	std::pair<std::function<void(T*&)>, std::function<void(CRefCount*&)>> del = std::make_pair(std::bind(&CMemaryPool::PoolDelete<T>, &pool, std::placeholders::_1), std::bind(&CMemaryPool::PoolDelete<CRefCount>, &pool, std::placeholders::_1));
	return CMemSharePtr<T>(o, ref, del);
}

template<typename T>
CMemSharePtr<T> MakeMallocSharedPtr(CMemaryPool& pool, int size) {
	T* o = (T*)pool.PoolMalloc<T>(size);
	CRefCount* ref = pool.PoolNew<CRefCount>();
	std::pair<std::function<void(T*&)>, std::function<void(CRefCount*&)>> del = std::make_pair(std::bind(&CMemaryPool::PoolFree<T>, &pool, std::placeholders::_1, size), std::bind(&CMemaryPool::PoolDelete<CRefCount>, &pool, std::placeholders::_1));
	return CMemSharePtr<T>(o, ref, del);
}

template<typename T>
CMemSharePtr<T> MakeLargeSharedPtr(CMemaryPool& pool) {
	T* o = pool.PoolLargeMalloc<T>();
	CRefCount* ref = pool.PoolNew<CRefCount>();
	std::pair<std::function<void(T*&)>, std::function<void(CRefCount*&)>> del = std::make_pair(std::bind(&CMemaryPool::PoolLargeFree<T>, &pool, std::placeholders::_1), std::bind(&CMemaryPool::PoolDelete<CRefCount>, &pool, std::placeholders::_1));
	return CMemSharePtr<T>(o, ref, del);
}

template<typename T>
CMemSharePtr<T> MakeLargeSharedPtr(CMemaryPool& pool, int size) {
	T* o = pool.PoolLargeMalloc<T>(size);
	CRefCount* ref = pool.PoolNew<CRefCount>();
	std::pair<std::function<void(T*&)>, std::function<void(CRefCount*&)>> del = std::make_pair(std::bind(&CMemaryPool::PoolLargeFree<T>, &pool, std::placeholders::_1, size), std::bind(&CMemaryPool::PoolDelete<CRefCount>, &pool, std::placeholders::_1));
	return CMemSharePtr<T>(o, ref, del);
}