#include "caculatePosition.h"
#include<iostream>
#include<cmath>
#include <iomanip>
#include <string.h>
#include <sstream>
#include <fstream>
#include <string>

void caculatePosition::caculateSatPos(std::vector<caculatePosition>& ca,double obsMoment)
{
	for (int num=0; num < ca.size(); num++)
	{
		//���������У�PRNΪ��ƥ���PRN�ţ�arrΪ��������ݾ��󣬰��������ļ���һЩ������obsMomentΪ�۲�ʱ��
		// ��Ҫע����ǣ��۲�ʱ��Ҳ��gps������
		//����黯ʱ���õ�
		//����滮ʱ��tk
		//���������˶���ƽ�����ٶ�n
		double GM = 3.986005 * pow(10, 14);
		double n, n0;
		n0 = (sqrt(GM)) / pow(ca[num].satliteData[1][3], 3);
		n = n0 + ca[num].satliteData[0][2];

		//��tocʱ��ת��ΪGPS������
		caculatePosition cax = ca[num].ctimeTOgpst(ca[num]);

		//����۲�ʱ�����ǵ�ƽ���ǵ�
		double obs_deltaT = obsMoment - cax.sow;
		double deltat = ca[num].TOC[0] + ca[num].TOC[1] * obs_deltaT + TOC[2] * pow(obs_deltaT, 2);
		double t = obsMoment - deltat;
		double tk = t - ca[num].satliteData[2][0];//����ط�����ԭ���е�tk=t-toe
		double Mk = ca[num].satliteData[0][3] + n * tk;

		//����ƫ�����
		//�˴����õ�������ȡƫ�����Ek
		const double pi = 3.14159265358979323846;
		Mk = std::remainder(Mk + 2 * pi, 2 * pi);
		double E = Mk;

		for (int i = 0; i < 10; i++) {
			int E_old = E;
			E = Mk + ca[num].satliteData[1][1] * std::sin(E);//ca.satliteData[1][1]Ϊ������
			double dE;
			dE = std::remainder(E - E_old, 2 * pi);
			if (std::abs(dE) < pow(10, -14))
			{
				break;
			}
		}
		//����������
		E = std::remainder(E + 2 * pi, 2 * pi);
		double e1 = 1 - pow(ca[num].satliteData[1][1], 2);
		double fs;
		fs = std::atan(sqrt(e1) * std::sin(E) / (std::cos(E) - ca[num].satliteData[1][1]));//�ţ����ݹ�ʽ���

		//���������Ǿ�
		double u0 = ca[num].satliteData[3][2] + fs;
		u0 = std::remainder(u0, 2 * pi);

		//�������Ǿ࣬����ʸ�������ǹ����ǽ����㶯����
		//�������㶯�������㷽��
		double sigema_u, sigema_r, sigema_i;
		double uk, rk, ik;
		sigema_u = ca[num].satliteData[1][0] * std::cos(2 * u0) + ca[num].satliteData[1][2] * std::sin(2 * u0);
		sigema_r = ca[num].satliteData[3][1] * std::cos(2 * u0) + ca[num].satliteData[0][1] * std::sin(2 * u0);
		sigema_i = ca[num].satliteData[2][1] * std::cos(2 * u0) + ca[num].satliteData[2][3] * std::sin(2 * u0);

		uk = u0 + sigema_u;
		rk = pow(ca[num].satliteData[1][3], 2) * (1 - ca[num].satliteData[1][1] * std::cos(E)) + sigema_r;
		ik = ca[num].satliteData[3][0] + sigema_i + ca[num].satliteData[4][0] * tk;

		//����۲�ʱ�̵������㾭��L
		double Omegak;
		double we = 7.2921151467 * pow(10, -5);
		Omegak = ca[num].satliteData[2][2] + (ca[num].satliteData[3][3] - we) * tk - we * ca[num].satliteData[2][0];

		//�����ڹ��ƽ�������ϵ�е�λ��
		double xk, yk;
		xk = rk * std::cos(uk);
		yk = rk * std::sin(uk);

		//����������WGS-84����ϵ�е�λ��
		double X, Y, Z;
		X = xk * std::cos(Omegak) - yk * std::cos(ik) * std::sin(Omegak);
		Y = xk * std::sin(Omegak) + yk * std::cos(ik) * std::cos(Omegak);
		Z = yk * std::sin(ik);

		if (isnan(X)==true)
		{
			ca[num].count = -1;
		}
		else
		{
			ca[num].count = num;
		}
		ca[num].deltat = deltat;
		std::cout <<ca[num].PRN<< "���ǵ�����Ϊ" << X << " " << Y << " " << Z << std::endl;
		ca[num].x = -X;
		ca[num].y = -Y;
		ca[num].z = -Z;
		
	}
}

caculatePosition caculatePosition::ctimeTOgpst(caculatePosition ca)
{
	int year = ca.TOC_year+2000;
	int month = ca.TOC_month;
	int day = ca.TOC_day;
	int hour = ca.TOC_hour;
	int minute = ca.TOC_minute;
	double second = ca.TOC_second;
	int DayofMonth = 0;
	int DayofYear = 0;
	int weekno = 0;
	int dayofweek;
	int m;

	//�����1980�굽��ǰ��ǰһ�������,DOY,day of year
	for (m = 1980; m < year; m++)
	{
		if ((m % 4 == 0 && m % 100 != 0) || (m % 400 == 0))
		{
			DayofYear += 366;
		}
		else
			DayofYear += 365;
	}
	//���㵱ǰһ���ڴ�Ԫ�µ���ǰǰһ�µ�����,DIY,day in year
	for (m = 1; m < month; m++)
	{
		if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12)
			DayofMonth += 31;
		else if (m == 4 || m == 6 || m == 9 || m == 11)
			DayofMonth += 30;
		else if (m == 2)
		{
			if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
				DayofMonth += 29;
			else
				DayofMonth += 28;

		}
	}
	DayofMonth = DayofMonth + day - 6;//���ϵ�������/��ȥ1980��Ԫ�µ�6��

	ca.gpsweek= (DayofYear + DayofMonth) / 7;//����GPS��,ȡ��
	dayofweek = (DayofYear + DayofMonth) % 7;//ȡ��,������
	//����GPSʱ��
	ca.sow = dayofweek * 86400 + hour * 3600 + minute * 60 + second;
	return ca;
}

double caculatePosition::ctimeTOgpsweek(int a[6])
{

	int year = a[0] + 2000;
	int month = a[1];
	int day = a[2];
	int hour = a[3];
	int minute = a[4];
	double second = a[5];
	int DayofMonth = 0;
	int DayofYear = 0;
	int weekno = 0;
	int dayofweek;
	int m;

	//�����1980�굽��ǰ��ǰһ�������,DOY,day of year
	for (m = 1980; m < year; m++)
	{
		if ((m % 4 == 0 && m % 100 != 0) || (m % 400 == 0))
		{
			DayofYear += 366;
		}
		else
			DayofYear += 365;
	}
	//���㵱ǰһ���ڴ�Ԫ�µ���ǰǰһ�µ�����,DIY,day in year
	for (m = 1; m < month; m++)
	{
		if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12)
			DayofMonth += 31;
		else if (m == 4 || m == 6 || m == 9 || m == 11)
			DayofMonth += 30;
		else if (m == 2)
		{
			if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
				DayofMonth += 29;
			else
				DayofMonth += 28;

		}
	}
	DayofMonth = DayofMonth + day - 6;//���ϵ�������/��ȥ1980��Ԫ�µ�6��

	double gpsweek = (DayofYear + DayofMonth) / 7;//����GPS��,ȡ��
	dayofweek = (DayofYear + DayofMonth) % 7;//ȡ��,������
	//����GPSʱ��
	double sow = dayofweek * 86400 + hour * 3600 + minute * 60 + second;

	return gpsweek;
}

double caculatePosition::ctimeTOgpsSec(int a[6])
{

	int year = a[0] + 2000;
	int month = a[1];
	int day = a[2];
	int hour = a[3];
	int minute = a[4];
	double second = a[5];
	int DayofMonth = 0;
	int DayofYear = 0;
	int weekno = 0;
	int dayofweek;
	int m;

	//�����1980�굽��ǰ��ǰһ�������,DOY,day of year
	for (m = 1980; m < year; m++)
	{
		if ((m % 4 == 0 && m % 100 != 0) || (m % 400 == 0))
		{
			DayofYear += 366;
		}
		else
			DayofYear += 365;
	}
	//���㵱ǰһ���ڴ�Ԫ�µ���ǰǰһ�µ�����,DIY,day in year
	for (m = 1; m < month; m++)
	{
		if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12)
			DayofMonth += 31;
		else if (m == 4 || m == 6 || m == 9 || m == 11)
			DayofMonth += 30;
		else if (m == 2)
		{
			if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
				DayofMonth += 29;
			else
				DayofMonth += 28;

		}
	}
	DayofMonth = DayofMonth + day - 6;//���ϵ�������/��ȥ1980��Ԫ�µ�6��

	double gpsweek = (DayofYear + DayofMonth) / 7;//����GPS��,ȡ��
	dayofweek = (DayofYear + DayofMonth) % 7;//ȡ��,������
	//����GPSʱ��
	double sow = dayofweek * 86400 + hour * 3600 + minute * 60 + second;

	return sow;
}



