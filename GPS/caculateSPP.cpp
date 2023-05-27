#include "caculateSPP.h"
#include<cmath>
#include"Matrix.h"
#include <iostream>

void caculatePPP::caculate(std::vector<caculatePosition>& ca, fileReader& fr, double X0, double Y0, double Z0)
{
	int n= size(ca);//��ȡ����
	double P0;//�����ֵ
	int countB=0;//B������
	double c = 3 * pow(10, 8);//����
	Matrix x(4, 1);//���������
	Matrix DOP(4, 4);
	for (int f = 0; f < 30; f++)
	{
		for (int i = 0; i < n; i++) {
			//����α���ֵ
			if (i == ca[i].count)
			{
				countB++;
			}
		}
		vector<vector<double>> B(countB, vector<double>(4));
		int col1 = 0;
		for (int i = 0; i < n; i++) {
			//����α���ֵ
			if (i == ca[i].count)
			{
				P0 = sqrt(pow(ca[i].x - X0, 2) + pow(ca[i].y - Y0, 2) + pow(ca[i].y - Y0, 2));
				B[col1][0] = ((-1) * (ca[i].x - X0)) / P0;
				B[col1][1] = ((-1) * (ca[i].y - Y0)) / P0;
				B[col1][2] = ((-1) * (ca[i].z - Z0)) / P0;
				B[col1][3] = 1;
				col1++;
			}
		}
		vector<double> L(countB);
		int col2 = 0;
		for (int i = 0; i < n; i++) {
			//����α���ֵ
			if (i == ca[i].count&&fr.getobsL1(fr, i)!=10000000)
			{
				P0 = sqrt(pow(ca[i].x - X0, 2) + pow(ca[i].y - Y0, 2) + pow(ca[i].z - Z0, 2));
				double abst = abs(fr.getobsL1(fr, i) - P0+c * ca[i].deltat);
				if ( abst> 150)
				{
					fr.setobsL1(fr, i, P0- c * ca[i].deltat+10);
				}
				L[col2] = fr.getobsL1(fr, i) - P0 + c * ca[i].deltat;
				col2++;
			}
		}
		Matrix MaB(countB, 4);
		Matrix MaL(countB, 1);

		//��B,L��ֵ��ֵ�����������
		for (int i = 0; i < countB; i++)
		{
			for (int j = 0; j < 4; j++) {
				MaB.setValue(i, j, B[i][j]);
			}
		}
		for (int i = 0; i < 1; i++)
		{
			for (int j = 0; j < 4; j++) {
				MaL.setValue(j, i, L[j]);
			}
		}

		int a = 1;
		x = ((MaB.transpose() * MaB).inverse()) * (MaB.transpose() * MaL);

		X0 = x.getValue(0, 0) + X0;
		Y0 = x.getValue(1, 0) + Y0;
		Z0 = x.getValue(2, 0) + Z0;

		if (x.getValue(0, 0) < 1 && x.getValue(1, 0) < 1 && x.getValue(2, 0) < 1)
		{
			//DOPֵ�ļ���
			DOP = (MaB.transpose() * MaB).inverse();
			break;
		}

	}

	for (int i = 0; i < 3; i++) {
		cout <<"����ƽ�����һ�ε����ĸ���������):" << x.getValue(i, 0) << endl;
	}
	cout << "α�൥�㶨λ���:" << X0 << " " << Y0 << " " << Z0 << endl;
	//λ�þ������ӵļ���
	double PDOP = sqrt(DOP.getValue(0, 0) + DOP.getValue(1, 1) + DOP.getValue(2, 2));
	cout << "λ�þ�������Ϊ��" << PDOP;
	//x = x.inv((MaB.T(MaB) * MaB)) * MaB.T(MaB) * MaL;
}
