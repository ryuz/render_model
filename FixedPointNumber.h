

#ifndef __RYUZ__FIXED_POINT_NUMBER_H__
#define __RYUZ__FIXED_POINT_NUMBER_H__

#include <assert.h>
#include <vector>
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

	FixedPointNumber(double v)
	{
		m_v = (T)(v * (1 << Q));
	}

	FixedPointNumber& operator=(const FixedPointNumber& obj)
	{
		m_v = obj.m_v;
		return *this;
	}

	FixedPointNumber& operator=(double v)
	{
		m_v = (T)(v * (1 << Q));
		return *this;
	}

	operator double() const
	{
		return (double)m_v / (double)(1 << Q);
	}
	
	FixedPointNumber operator+(FixedPointNumber& obj) const
	{
		FixedPointNumber t;
		t.m_v = m_v + obj.m_v;
		return t;
	}

	FixedPointNumber operator-(FixedPointNumber& obj) const
	{
		FixedPointNumber t;
		t.m_v = m_v - obj.m_v;
		return t;
	}

	FixedPointNumber operator*(FixedPointNumber& obj) const
	{
		FixedPointNumber t;
		t.m_v = (T)(((LT)m_v * (LT)obj.m_v) >> Q);
		return t;
	}

	FixedPointNumber operator/(FixedPointNumber& obj) const
	{
		FixedPointNumber t;
		t.m_v = (T)(((LT)m_v << Q) / (LT)obj.m_v);
		return t;
	}
};


#endif	// __RYUZ__FIXED_POINT_NUMBER_H__
