#pragma once
#include<vector>
#include<string>
#include <string.h>
#include <string>
class caculatePosition
{
public:
	caculatePosition() {
		
	}

	//�����ʾһ���ض����ǵ�PRN����
	std::string PRN;
	int count;
	double deltat;//�����Ӳƽ���ʱ��Ҫ��
	//�����ʾ�����ӵ�����time of clock�������Ӳ��
	double TOC[3];
	//�����ʾ�����ļ����������ݣ�����ο����ϵ����ݽ��;��У���Ҫ�ĸ����ĸ�
	double satliteData[6][4];
	//�����ʾ�����ļ������һ�е������������ķ���ʱ�����������
	double satliteDataElse[2];

	//TOCʱ�̣���������Ҳ��֪�������ʲôʱ��
	int TOC_year;
	int TOC_month;
	int TOC_day;
	int TOC_hour;
	int TOC_minute;
	double TOC_second;

	//GPS����GPS������
	long gpsweek;
	double sow;

	void caculateSatPos(std::vector<caculatePosition>& ca, double obsMoment);
	caculatePosition ctimeTOgpst(caculatePosition ca);
	double ctimeTOgpsweek(int a[6]);
	double ctimeTOgpsSec(int a[6]);

	double x;
	double y;
	double z;

};

//λ��//�ѿ�������
struct postion
{
	double X, Y, Z;
};
//λ��//�������ϵ
struct posblh
{
	double B, L, H;
};
//λ��//վ������ϵ
struct posenu
{
	double E, N, U;
};

