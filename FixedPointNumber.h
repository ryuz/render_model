

#ifndef __RYUZ__FIXED_POINT_NUMBER_H__
#define __RYUZ__FIXED_POINT_NUMBER_H__

#include <assert.h>
#include <vector>
#include <iostream>
#include "opencv2/opencv.hpp"


template <int Q=12, int N=32, typename T=int, typename LT=long long>
class FixedPointNumber
{
protected:
	T	m_v;

public:
	FixedPointNumber()
	{
		m_v = 0;
	}

	FixedPointNumber(const FixedPointNumber& obj)
	{
		m_v = obj.m_v;
	}

	T	GetRawValue(void) { return m_v; }

	inline FixedPointNumber(double v)
	{
		m_v = (T)(v * (1 << Q));
	}

	inline FixedPointNumber(float v)
	{
		m_v = (T)(v * (1 << Q));
	}

	inline FixedPointNumber(int v)
	{
		m_v = (T)((T)v << Q);
	}

	inline FixedPointNumber& operator=(const FixedPointNumber& obj)
	{
		m_v = obj.m_v;
		return *this;
	}

	inline FixedPointNumber& operator=(double v)
	{
		m_v = (T)(v * (1 << Q));
		return *this;
	}

	inline operator double() const
	{
		return (double)m_v / (double)(1 << Q);
	}

	inline operator float() const
	{
		return (float)m_v / (float)(1 << Q);
	}

	inline operator int() const
	{
		return (int)(m_v >> Q);
	}
	
	inline FixedPointNumber operator+() const
	{
		FixedPointNumber t;
		t.m_v = +m_v;
		return t;
	}

	inline FixedPointNumber operator-() const
	{
		FixedPointNumber t;
		t.m_v = -m_v;
		return t;
	}

	inline FixedPointNumber operator+(const FixedPointNumber& obj) const
	{
		FixedPointNumber t;
		t.m_v = m_v + obj.m_v;
		return t;
	}
	
	inline FixedPointNumber& operator+=(const FixedPointNumber& obj)
	{
		m_v += obj.m_v;
		return *this;
	}

	inline FixedPointNumber operator-(const FixedPointNumber& obj) const
	{
		FixedPointNumber t;
		t.m_v = m_v - obj.m_v;
		return t;
	}

	inline FixedPointNumber& operator-=(const FixedPointNumber& obj)
	{
		m_v -= obj.m_v;
		return *this;
	}

	inline FixedPointNumber operator*(const FixedPointNumber& obj) const
	{
		FixedPointNumber t;
		t.m_v = (T)(((LT)m_v * (LT)obj.m_v) >> Q);
		return t;
	}

	inline FixedPointNumber& operator*=(const FixedPointNumber& obj)
	{
		m_v = (T)(((LT)m_v * (LT)obj.m_v) >> Q);
		return *this;
	}

	inline FixedPointNumber operator/(const FixedPointNumber& obj) const
	{
		FixedPointNumber t;
		t.m_v = (T)(((LT)m_v << Q) / (LT)obj.m_v);
		return t;
	}

	inline FixedPointNumber& operator/=(const FixedPointNumber& obj)
	{
		m_v = (T)(((LT)m_v << Q) / (LT)obj.m_v);
		return *this;
	}
	
	inline bool operator>=(const FixedPointNumber& obj) const
	{
		return (m_v >= obj.m_v);
	}
};


#endif	// __RYUZ__FIXED_POINT_NUMBER_H__
