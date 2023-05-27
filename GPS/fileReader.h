#pragma once
#include <string>
#include <string.h>
#include"caculatePosition.h"
#include<vector>
using namespace std;
class fileReader
{
public:
	//���캯��������
	fileReader() {

	}
	fileReader(string filepath,string filepath_n) 
	{
		this->filepath = filepath;
		this->filepath_n = filepath_n;
	}
	/*�����ȡ��д��O�ļ���N�ļ�*/
	void readFile_O(struct obs_head& ohead, struct obs_epoch& oepoch, string broadcastFile, fileReader& filereader);
	void readFile_N(std::vector<struct navi_body>& nbody, string navigationFile, fileReader& filereader, int allHangNum_n);
	int getHeaderNum(string navigationFile);
	string getLine(const std::string& fileName, int lineNumber);

	//���ṹ��navi_body�����ݴ���caculatePosition������
	std::vector<caculatePosition> nbodyTocacupos(std::vector<struct navi_body>& nbody, std::vector<caculatePosition>& ca);

	void nbodyTonbody(struct navi_body& nbody1, struct navi_body& nbody2);

	//��ȡ���ǵ�PRN��,���У��ڶ���Ϊ���ǵ�����
	void getPRN(struct obs_epoch& oepoch, int num, std::vector<int>& vec);

	//��ȡα��,��ȡ����һ�е�α��
	double getobsL1(fileReader fr,int col);

	void setobsL1(fileReader& fr, int col, double N);

	//��ȡ���ǵ�����,���ڶ���Ϊ���ǵ�����
	void getSatliteType(struct obs_epoch& oepoch, int num, std::vector<char>& vec);

	//��ȡ��ʼ�����꣨�������꣩
	int getInitPosition(string fileName);

	//��ȡ�ļ�ʱȥ���ļ��еĿո�,���Ϊ�ո�ֵ��10000000��
	string trim(string& s);
	//������������Ҫ��Ϊ��ʵ�ֶ�ȡ�ض���
	//������Ҳ��֪����ôʵ�ֵģ����ϳ���
	int CountLines(string filename);
	string ReadLine(string filename, int line);
private:
	string filepath;
	string filepath_n;
	//����һ����Ԫ����������
	double observation[25][8];
};
//�۲�ֵͷ�ļ��Ľṹ��
typedef struct obs_head
{
	double ver;//RINEX�ļ��汾��
	string type;//�ļ�����
	double apX;//��վ����λ��XYZ��WGS84��
	double apY;
	double apZ;
	int obstypenum;//�۲�ֵ��������
	string obstype;//rinex�۲�ֵ�����б�
	double interval;//�۲�ֵ����Ԫ���
	int f_y;//��һ���۲��¼��ʱ��
	int f_m;
	int f_d;
	int f_h;
	int f_min;
	double f_sec;
	char tsys[5];//ʱ��ϵͳ
};


//�۲�ֵ��Ԫ���ݽṹ��
typedef struct obs_epoch
{
	//�۲���Ԫʱ��
	int y;
	int m;
	int d;
	int h;
	int min;
	double sec;
	int p_flag;//��Ԫ��־
	int sat_num;//��ǰ��Ԫ���۲⵽����������
	//�˴����嶯̬���飬����ָ�룬��Ϊ�����ڶ�ȡ�����ݣ��Ҳ�֪����������ڴ�
	string satName;//�ַ������վ���ƴ洢
};

typedef struct navi_body
{
	//�����ʾһ���ض����ǵ�PRN����
	string PRN;
	//�����ʾ�����Ӵ�����time of clock
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
};


