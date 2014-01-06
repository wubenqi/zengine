// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef BASE2_SHARED_PTR_H_
#define BASE2_SHARED_PTR_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "base/logging.h"

namespace base {

// For SharedPtr
class RefCounted2 :
	public subtle::RefCountedBase {
public:
	RefCounted2() { 
		AddRef();
	}
	~RefCounted2() { }

	void AddRef() const {
		subtle::RefCountedBase::AddRef();
	}

	// Returns true if the object should self-delete.
	bool Release() const {
		return subtle::RefCountedBase::Release();
	}

private:
	DISALLOW_COPY_AND_ASSIGN(RefCounted2);
};

class RefCountedThreadSafe2 :
	public subtle::RefCountedThreadSafeBase {
public:
	RefCountedThreadSafe2() { 
		AddRef();
	}
	~RefCountedThreadSafe2() { }

	void AddRef() const {
		subtle::RefCountedThreadSafeBase::AddRef();
	}

	// Returns true if the object should self-delete.
	bool Release() const {
		return subtle::RefCountedThreadSafeBase::Release();
	}

private:
	DISALLOW_COPY_AND_ASSIGN(RefCountedThreadSafe2);
};

// The default release policy for SharedPtr, which
// simply uses the delete operator to delete an object.
template <class T>
class ReleaseDeletePolicy {
public:
	// Delete the object.
	// Note that pObj can be 0.
	static void release(T* pObj) {
		delete pObj;
	}
};


// The release policy for SharedPtr holding arrays.
template <class T>
class ReleaseDeleteArrayPolicy {
public:
	// Delete the object.
	// Note that pObj can be 0.
	static void release(T* pObj) {
		delete [] pObj;
	}
};

// The release policy for SharedPtr holding arrays.
template <class T>
class ReleaseFreePolicy {
public:
	// Delete the object.
	// Note that pObj can be 0.
	static void release(T* pObj) {
		free(pObj);
	}
};

template <class T, class RefCounted = RefCountedThreadSafe2, class ReleasePolicy = ReleaseDeletePolicy<T> >
class SharedPtr {
public:
	SharedPtr()
		: ref_counted_(new RefCounted)
		, ptr_(0) {
		CHECK(ref_counted_);
		// ref_counted_->AddRef();
	}

	explicit SharedPtr(T* ptr)
		: ref_counted_(new RefCounted)
		, ptr_(ptr) {
		CHECK(ref_counted_);
		// ref_counted_->AddRef();
	}

	template <class OtherT, class OtherReleasePolicy> 
	SharedPtr(const SharedPtr<OtherT, RefCounted, OtherReleasePolicy>& ptr)
		: ref_counted_(ptr.ref_counted_)
		, ptr_(const_cast<OtherT*>(ptr.get())) {
		ref_counted_->AddRef();
	}

	SharedPtr(const SharedPtr& ptr)
		: ref_counted_(ptr.ref_counted_)
		, ptr_(ptr.ptr_) {
		ref_counted_->AddRef();
	}

	~SharedPtr() {
		release();
	}

	SharedPtr& assign(T* ptr) {
		if (get() != ptr) {
			RefCounted* ref_tmp = new RefCounted;
			release();
			ref_counted_ = ref_tmp;
			ptr_ = ptr;
		}
		return *this;
	}

	SharedPtr& assign(const SharedPtr& ptr) {
		if (&ptr != this) {
			SharedPtr tmp(ptr);
			swap(tmp);
		}
		return *this;
	}
	
	template <class OtherT, class OtherReleasePolicy>
	SharedPtr& assign(const SharedPtr<OtherT, RefCounted, OtherReleasePolicy>& ptr) {
		if (ptr.get() != ptr_) {
			SharedPtr tmp(ptr);
			swap(tmp);
		}
		return *this;
	}

	SharedPtr& operator = (T* ptr) {
		return assign(ptr);
	}

	SharedPtr& operator = (const SharedPtr& ptr) {
		return assign(ptr);
	}

	template <class OtherT, class OtherReleasePolicy>
	SharedPtr& operator = (const SharedPtr<OtherT, RefCounted, OtherReleasePolicy>& ptr) {
		return assign<OtherT, OtherReleasePolicy>(ptr);
	}

	void swap(SharedPtr& ptr) {
		std::swap(ptr_, ptr.ptr_);
		std::swap(ref_counted_, ptr.ref_counted_);
	}

	template <class OtherT> 
	SharedPtr<OtherT, RefCounted, ReleasePolicy> cast() const {
		OtherT* other = dynamic_cast<OtherT*>(ptr_);
		if (other)
			return SharedPtr<OtherT, RefCounted, ReleasePolicy>(ref_counted_, other);
		return SharedPtr<OtherT, RefCounted, ReleasePolicy>();
	}

	template <class OtherT> 
	SharedPtr<OtherT, RefCounted, ReleasePolicy> unsafe_cast() const {
		OtherT* other = static_cast<OtherT*>(ptr_);
		return SharedPtr<OtherT, RefCounted, ReleasePolicy>(ref_counted_, other);
	}

	T* operator -> () {
		return deref();
	}

	const T* operator -> () const {
		return deref();
	}

	T& operator * () {
		return *deref();
	}

	const T& operator * () const {
		return *deref();
	}

	T* get() {
		return ptr_;
	}

	const T* get() const {
		return ptr_;
	}

	operator T* () {
		return ptr_;
	}
	
	operator const T* () const {
		return ptr_;
	}

	bool operator ! () const {
		return ptr_ == 0;
	}

	bool IsNull() const {
		return ptr_ == 0;
	}

	bool operator == (const SharedPtr& ptr) const {
		return get() == ptr.get();
	}

	bool operator == (const T* ptr) const {
		return get() == ptr;
	}

	bool operator == (T* ptr) const {
		return get() == ptr;
	}

	bool operator != (const SharedPtr& ptr) const {
		return get() != ptr.get();
	}

	bool operator != (const T* ptr) const {
		return get() != ptr;
	}

	bool operator != (T* ptr) const {
		return get() != ptr;
	}
	
private:
	T* deref() const {
		CHECK(ptr_);
		return ptr_;
	}

	void release() {
		CHECK(ref_counted_);
		bool _release = ref_counted_->Release();
		if (_release) {
			ReleasePolicy::release(ptr_);
			ptr_ = 0;

			delete ref_counted_;
			ref_counted_ = 0;
		}
	}

	// for cast operation
	SharedPtr(RefCounted* ref, T* ptr)
		: ref_counted_(ref)
		, ptr_(ptr) {
		CHECK(ref_counted_);
		ref_counted_->Release();
	}

private:
	RefCounted* ref_counted_;
	T*  ptr_;

	template <class OtherT, class OtherRefCounted, class OtherReleasePolicy> friend class SharedPtr;
};


template <class T, class RefCounted, class ReleasePolicy>
inline void swap(SharedPtr<T, RefCounted, ReleasePolicy>& p1, SharedPtr<T, RefCounted, ReleasePolicy>& p2) {
	p1.swap(p2);
}


} // namespace base


#endif // 
