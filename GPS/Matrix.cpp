#include "Matrix.h"
#include"SDKDDKVer.h"
#include "math.h"
#include <assert.h>
#include <atlstr.h>
#include <cstringt.h>
#include <iostream>
#include <tchar.h> 

// Ĭ�Ϲ��캯��
Matrix::Matrix(void)
	: _A(NULL)
	, _row(-1)
	, _column(-1)
{
}

// ���캯��һ
Matrix::Matrix(int row, int column)
{
	_row = row;
	_column = column;

	_A = new double* [row];
	for (int i = 0; i < row; i++)
		_A[i] = new double[column];

	// Initialize
	for (int i = 0; i < row; i++)
		for (int j = 0; j < column; j++)
			_A[i][j] = 0.0;
}

// ���ƹ��캯��
Matrix::Matrix(const Matrix& m)
{
	//if (_row > 0)
	//{
	//	for (int i = 0; i < _row; i++)
	//		delete [] _A[i];
	//	
	//	//if (NULL != _A)
	//	delete [] _A;
	//}

	_row = m._row;
	_column = m._column;

	_A = new double* [_row];
	for (int i = 0; i < _row; i++)
		_A[i] = new double[_column];

	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _column; j++)
		{
			_A[i][j] = m._A[i][j];
		}
	}
}

// Ĭ����������
Matrix::~Matrix(void)
{
	;
}

// ��ֵ�����
Matrix& Matrix::operator=(const Matrix& m)
{
	if (&m == this)
		return *this;

	// Added by ybwang at 2014/06/04
	if (NULL != _A)
	{
		for (int i = 0; i < _row; i++)
			delete[] _A[i];

		delete[] _A;
	}

	_row = m._row;
	_column = m._column;

	_A = new double* [_row];
	for (int i = 0; i < _row; i++)
		_A[i] = new double[_column];

	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _column; j++)
		{
			_A[i][j] = m._A[i][j];
		}
	}

	return *this;
}

// ���õ�λ��
bool Matrix::setUnitMatrix()
{
	if (_row != _column)
		return false;

	for (int i = 0; i < _row; ++i)
	{
		for (int j = 0; j < _column; ++j)
		{
			if (i == j)
				_A[i][j] = 1.0f;
			else
				_A[i][j] = 0.0f;
		}
	}

	return true;
}

// �Ƚ������
bool Matrix::operator==(const Matrix& m)
{
	if ((_row != m.getRow()) || (_column != m.getColumn())) return false;

	for (int i = 0; i < _row; i++)
		for (int j = 0; j < _column; j++)
			if (fabs(_A[i][j] - m.getValue(i, j)) > 10e-6)
				return false;

	return true;
}
bool Matrix::operator!=(const Matrix& m)
{
	if ((_row != m.getRow()) || (_column != m.getColumn())) return true;

	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _column; j++)
		{
			if (fabs(_A[i][j] - m.getValue(i, j)) < 10e-6)
				continue;
			else
				return true;
		}
	}

	return false;
}

// �ӷ������
Matrix Matrix::operator+(const Matrix& m)
{
	assert(_row == m.getRow() && _column == m.getColumn());

	Matrix result(_row, _column);

	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _column; j++)
		{
			result.setValue(i, j, _A[i][j] + m.getValue(i, j));
		}
	}

	return result;
}

// ���������
Matrix Matrix::operator-(const Matrix& m)
{
	assert(_row == m.getRow() && _column == m.getColumn());

	Matrix result(_row, _column);

	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _column; j++)
		{
			result.setValue(i, j, _A[i][j] - m.getValue(i, j));
		}
	}

	return result;
}

// �Լ������
Matrix& Matrix::operator +=(const Matrix& m)
{
	assert(_row == m.getRow() && _column == m.getColumn());

	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _column; j++)
		{
			_A[i][j] += m.getValue(i, j);
		}
	}

	return *this;
}

// �Լ������
Matrix& Matrix::operator-=(const Matrix& m)
{
	assert(_row == m.getRow() && _column == m.getColumn());

	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _column; j++)
			_A[i][j] -= m.getValue(i, j);
	}

	return *this;
}

// �˷�����
Matrix Matrix::operator*(const Matrix& m)
{
	assert(_column == m.getRow());

	Matrix result(_row, m.getColumn());
	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < result.getColumn(); j++)
		{
			double value = 0.0f;
			for (int k = 0; k < getColumn(); k++)	// ����֮ǰ�����˴���
				value += getValue(i, k) * m.getValue(k, j);

			result.setValue(i, j, value);
		}
	}
	return result;
}
Matrix Matrix::operator*(const double& s)
{
	Matrix result(_row, _column);

	for (int i = 0; i < _row; ++i)
	{
		for (int j = 0; j < _column; ++j)
		{
			result.setValue(i, j, getValue(i, j) * s);
		}
	}

	return result;
}

// ��Ŀ�������ȡ��
Matrix Matrix::operator-()
{
	Matrix m(_row, _column);

	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _column; j++)
			m.setValue(i, j, -_A[i][j]);
	}

	return m;
}

// ת��
Matrix Matrix::transpose()
{
	Matrix m(_column, _row);

	for (int i = 0; i < _column; i++)
	{
		for (int j = 0; j < _row; j++)
		{
			m.setValue(i, j, getValue(j, i));
		}
	}

	return m;
}

// ��������(����Ԫ�ط�)
Matrix Matrix::inverse()
{
	assert(_row == _column);
	Matrix NXW(_row, _row * 2);
	// ����NXW��ֵ
	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _row * 2; j++)
		{
			if (j < _row) NXW.setValue(i, j, getValue(i, j));
			else
			{
				if (i == (j - _row)) NXW.setValue(i, j, 1.0f);
				else NXW.setValue(i, j, 0.0f);
			}
		}
	}

	//traceMatrix(NXW);

	// �ø�˹-������ȥ�����ƽ�淽�̵�ϵ����ֻҪ��NTW������ȥ�������ϵ��
	long kindex;
	double mk; // mkΪ����Ԫ��ֵ��kindex���ڴ洢����Ԫ���ڵ�λ�ã��У�
	double aiitem; // ����Ѱ������Ԫʱ��ʱ�洢���ֵ

	for (int i = 0; i < _row; i++)
	{
		// ѡȡ����Ԫ
		mk = NXW.getValue(i, i);
		kindex = -1;
		for (int j = i + 1; j < _row; j++)
		{
			if (fabs(NXW.getValue(j, i)) > fabs(mk)) // ��Ҫ���Ǿ���ֵ������ 
			{
				mk = NXW.getValue(j, i);
				kindex = j;
			}
		}

		// ����Ԫѡȡ���
		// ������
		if (kindex != -1)
		{
			for (int k = 0; k < _row * 2; k++)
			{
				aiitem = NXW.getValue(i, k);
				NXW.setValue(i, k, NXW.getValue(kindex, k));
				NXW.setValue(kindex, k, aiitem);
			}
		}

		// ���������
		// ��ȥ
		for (int k = 0; k < _row * 2; k++)
			NXW.setValue(i, k, NXW.getValue(i, k) / mk); // ���õ�ǰ�е�����ԪΪ1.0
		for (int k = 0; k < _row; k++)
		{
			if (k == i)
				continue;
			double tem = NXW.getValue(k, i); // ��ΪNXW[k][i]��ֵ��ʱ�仯�����Ա���������ʱ�����Ա��ָ�ֵ
			for (int l = 0; l < _row * 2; l++)
			{
				NXW.setValue(k, l, NXW.getValue(k, l) - tem * NXW.getValue(i, l));
			}
		}

		//TRACE("New\n");
		//traceMatrix(NXW);
	}

	//TRACE("Matrix NXW:\n");
	//traceMatrix(NXW);

	Matrix result(_row, _row);
	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _row; j++)
		{
			result.setValue(i, j, NXW.getValue(i, j + _row));
		}
	}

	return result;
}


