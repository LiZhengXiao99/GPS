#pragma once
#define _USE_MATH_DEFINES
#include <string>
#include <math.h>
using namespace std;
class Matrix
{
public:
	// Ĭ�Ϲ��캯��
	Matrix(void);
	// ���캯��һ
	Matrix(int row, int column);
	// ���ƹ��캯��
	Matrix(const Matrix& m);

public:
	// Ĭ����������
	~Matrix(void);

public:	// �����
	// ��ֵ�����
	Matrix& operator=(const Matrix& m);
	// �Ƚ������
	bool operator==(const Matrix& m);
	bool operator!=(const Matrix& m);
	// ��/�������
	Matrix operator+(const Matrix& m);
	Matrix operator-(const Matrix& m);

	// �Լ�/�������
	Matrix& operator+=(const Matrix& m);
	Matrix& operator-=(const Matrix& m);

	// ��Ŀ�����
	Matrix operator-();	// ȡ����

	// �˷������
	Matrix operator*(const Matrix& m);
	Matrix operator*(const double& s);


public:	// ���Ա���
	// ����(i,j)��ֵ
	void setValue(int row, int column, double value) { _A[row][column] = value; }
	double getValue(int row, int column) const { return _A[row][column]; }

	// �����С��е�ֵ
	void setRow(const int row) { _row = row; }
	int getRow() const { return _row; }
	void setColunm(const int column) { _column = column; }
	int getColumn() const { return _column; }

	bool setUnitMatrix();

public:
	// ����ת��
	Matrix transpose();
	// ��������
	Matrix inverse();	// ����Ԫ�ط�
private:
	double** _A;
	int _row;	// ��
	int _column;	// ��
};