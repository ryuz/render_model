

#ifndef __RYUZ__VECTOR_H__
#define __RYUZ__VECTOR_H__


#include <assert.h>
#include <vector>


template <typename T>
class Vector
{
public:
	Vector() {}
	Vector(unsigned int size) : m_v(size) {}
	Vector(unsigned int size, T val) : m_v(size, val) {}
	
	Vector(const Vector& obj) { *this = obj; }

	void			Resize(unsigned int size)	{ return m_v.resize(size); }
	unsigned int	Size(void) const			{ return m_v.size(); }

	T&	operator[](unsigned int n)	{ return m_v[n]; }

	// ���
	inline Vector&	operator=(const Vector& obj)
	{
		m_v = obj.m_v;
		return *this;
	}
	
	// �P���}�C�i�X
	inline Vector	operator-() const
	{
		Vector	v(Size());
		for ( unsigned int i = 0; i < Size(); i++ ) {
			v.m_v[i] = -m_v[i];
		}
		return v;
	}

	// ���Z
	inline Vector	operator+(const Vector& obj) const
	{
		assert(Size() == obj.Size());
		Vector	v(Size());
		for ( unsigned int i = 0; i < Size(); i++ ) {
			v.m_v[i] = m_v[i] + obj.m_v[i];
		}
		return v;
	}
	inline Vector&	operator+=(const Vector& obj) { *this = *this + obj; return *this; }
	
	// �X�J�����Z
	inline Vector	operator+(T a) const
	{
		Vector	v(Size());
		for ( unsigned int i = 0; i < Size(); i++ ) {
			v.m_v[i] = m_v[i] + a;
		}
		return v;
	}
	inline Vector&	operator+=(T a) { *this = *this + a; return *this; }

	// ���Z
	inline Vector	operator-(const Vector& obj) const
	{
		assert(Size() == obj.Size());
		Vector	v(Size());
		for ( unsigned int i = 0; i < Size(); i++ ) {
			v.m_v[i] = m_v[i] - obj.m_v[i];
		}
		return v;
	}
	inline Vector&	operator-=(const Vector& obj) { *this = *this - obj; return *this; }

	// �X�J�����Z
	inline Vector	operator-(T a) const
	{
		Vector	v(Size());
		for ( unsigned int i = 0; i < Size(); i++ ) {
			v.m_v[i] = m_v[i] - a;
		}
		return v;
	}
	inline Vector&	operator-=(T a) { *this = *this - a; return *this; }

	// ��Z
	inline Vector	operator*(const Vector& obj) const
	{
		assert(Size() == obj.Size());
		Vector	v(Size());
		for ( unsigned int i = 0; i < Size(); i++ ) {
			v.m_v[i] = m_v[i] * obj.m_v[i];
		}
		return v;
	}
	inline Vector&	operator*=(const Vector& obj) { *this = *this * obj; return *this; }

	// �X�J����Z
	inline Vector	operator*(T a) const
	{
		Vector	v(Size());
		for ( unsigned int i = 0; i < Size(); i++ ) {
			v.m_v[i] = m_v[i] * a;
		}
		return v;
	}
	inline Vector&	operator*=(T a) { *this = *this * a; return *this; }

	// ���Z
	inline Vector	operator/(const Vector& obj) const
	{
		assert(Size() == obj.Size());
		Vector	v(Size());
		for ( unsigned int i = 0; i < Size(); i++ ) {
			v.m_v[i] = m_v[i] / obj.m_v[i];
		}
		return v;
	}
	inline Vector&	operator/=(const Vector& obj) { *this = *this / obj; return *this; }

	// �X�J�����Z
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
	std::vector<T>	m_v;	
};


#endif	// __RYUZ__VECTOR_H__


// end of file
