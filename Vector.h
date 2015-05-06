

#ifndef __RYUZ__VECTOR_H__
#define __RYUZ__VECTOR_H__


#include <assert.h>
#include <vector>

#ifndef _ASSERT
#ifdef DEBUG
#define _ASSERT(x)	assert(x)
#else
#define _ASSERT(x)	do{}while()
#endif 
#endif


template <typename T>
class Vector
{
public:
	Vector()
	{
		m_size = 0;
		m_v    = new T[m_size];
	}
	
	Vector(unsigned int size)
	{
		m_size = size;
		m_v    = new T[m_size];
	}

	Vector(unsigned int size, T val)
	{
		m_size = size;
		m_v    = new T[m_size];
		for ( unsigned int i = 0; i < m_size; i++ ) {
			m_v[i] = val;
		}
	}
	
	Vector(const Vector& obj)
	{
		m_size = obj.Size();
		m_v    = new T[m_size];
		for ( unsigned int i = 0; i < m_size; i++ ) {
			m_v[i] = obj.m_v[i];
		}
	}

	~Vector()
	{
		delete[] m_v;
	}

	inline unsigned int	Size(void) const		{ return m_size; }

	void Resize(unsigned int size)
	{
		delete[] m_v;
		m_size = size;
		m_v    = new T[m_size];
	}
	

	T&	operator[](unsigned int n)	{ return m_v[n]; }

	// ‘ã“ü
	inline Vector&	operator=(const Vector& obj)
	{
		Resize(obj.Size());
		for ( unsigned int i = 0; i < m_size; i++ ) {
			m_v[i] = obj.m_v[i];
		}
		return *this;
	}
	
	// ’P€ƒ}ƒCƒiƒX
	inline Vector	operator-() const
	{
		Vector	v(Size());
		for ( unsigned int i = 0; i < Size(); i++ ) {
			v.m_v[i] = -m_v[i];
		}
		return v;
	}

	// ‰ÁŽZ
	inline Vector operator+(const Vector& obj) const
	{
		_ASSERT(Size() == obj.Size());
		Vector	v(Size());
		for ( unsigned int i = 0; i < Size(); i++ ) {
			v.m_v[i] = m_v[i] + obj.m_v[i];
		}
		return v;
	}

	// ‰ÁŽZ‘ã“ü
	inline Vector& operator+=(const Vector& obj)
	{
		*this = *this + obj; return *this;
	}
	
	// ƒXƒJƒ‰‰ÁŽZ
	inline Vector	operator+(T a) const
	{
		Vector	v(Size());
		for ( unsigned int i = 0; i < Size(); i++ ) {
			v.m_v[i] = m_v[i] + a;
		}
		return v;
	}
	inline Vector&	operator+=(T a) { *this = *this + a; return *this; }

	// Œ¸ŽZ
	inline Vector	operator-(const Vector& obj) const
	{
		_ASSERT(Size() == obj.Size());
		Vector	v(Size());
		for ( unsigned int i = 0; i < Size(); i++ ) {
			v.m_v[i] = m_v[i] - obj.m_v[i];
		}
		return v;
	}
	inline Vector&	operator-=(const Vector& obj) { *this = *this - obj; return *this; }

	// ƒXƒJƒ‰Œ¸ŽZ
	inline Vector	operator-(T a) const
	{
		Vector	v(Size());
		for ( unsigned int i = 0; i < Size(); i++ ) {
			v.m_v[i] = m_v[i] - a;
		}
		return v;
	}
	inline Vector&	operator-=(T a) { *this = *this - a; return *this; }

	// æŽZ
	inline Vector	operator*(const Vector& obj) const
	{
		_ASSERT(Size() == obj.Size());
		Vector	v(Size());
		for ( unsigned int i = 0; i < Size(); i++ ) {
			v.m_v[i] = m_v[i] * obj.m_v[i];
		}
		return v;
	}
	inline Vector&	operator*=(const Vector& obj) { *this = *this * obj; return *this; }

	// ƒXƒJƒ‰æŽZ
	inline Vector	operator*(T a) const
	{
		Vector	v(Size());
		for ( unsigned int i = 0; i < Size(); i++ ) {
			v.m_v[i] = m_v[i] * a;
		}
		return v;
	}
	inline Vector&	operator*=(T a) { *this = *this * a; return *this; }

	// œŽZ
	inline Vector	operator/(const Vector& obj) const
	{
		_ASSERT(Size() == obj.Size());
		Vector	v(Size());
		for ( unsigned int i = 0; i < Size(); i++ ) {
			v.m_v[i] = m_v[i] / obj.m_v[i];
		}
		return v;
	}
	inline Vector&	operator/=(const Vector& obj) { *this = *this / obj; return *this; }

	// ƒXƒJƒ‰œŽZ
	inline Vector	operator/(T a) const
	{
		Vector	v(Size());
		for ( unsigned int i = 0; i < Size(); i++ ) {
			v.m_v[i] = m_v[i] / a;
		}
		return v;
	}
	inline Vector&	operator/=(T a) { *this = *this / a; return *this; }

protected:
	T*				m_v;
	unsigned int	m_size;
};


#endif	// __RYUZ__VECTOR_H__


// end of file
