#include<iostream>
#include<cmath>
#include <iomanip>
#include "fileReader.h"
#include <string.h>
#include <sstream>
#include <fstream>
#include <string>

//O�ļ��Ķ�ȡ
void fileReader::readFile_O(struct obs_head& ohead, struct obs_epoch& oepoch, string broadcastFile, fileReader& filereader)
{
	//��ȡ����
	string fileName = broadcastFile;

	/*�����ļ�������*/
	fstream file;

	//�����ļ�
	file.open(fileName);

	//�ж��Ƿ��ҵ��ļ�
	if (!file)
	{
		std::cout << "�ļ���ַ����������Ѱ���ļ�" << endl;
	}

	//��ʱ�洢���ݵ�ÿһ�У������ַ����飬��Ȼ���治�ö���
	string buf;
	//�ļ�������
	int allHangNum = 0;
	//whileѭ����Ҫ��ͷ�����ݵĶ�ȡ
	while (1)
	{
		//��ȡ���ļ���һ��
		getline(file, buf);
		//��ȡ��һ������֮�󣬿���������������¼һ��
		allHangNum++;
		//����ҵ���"END OF HEADER"������ѭ��,�������еļ�¼
		if (buf.find("END OF HEADER") != std::string::npos)
			break;
		//����ͷ�ṹ�����������ȡ
		if (buf.find("RINEX VERSION / TYPE") == 60)
		{
			//���ַ����еõ��汾�ţ�ͬʱһ��Ҫע��ת����double���ͣ���Ϊ�ṹ���ж������double����
			ohead.ver = std::stod(buf.substr(5, 9));
			//���ַ����еõ��۲�ֵ���ͣ�����M�ʹ����Ϲ۲�ֵ
			ohead.type = buf.substr(20, 36);
			//�������
			std::cout << "�汾�ţ�" << ohead.ver << endl;
			std::cout << "�۲�ֵ���ͣ�" << ohead.type << endl;
		}

		//��ȡ��������X,Y,Z
		if (buf.find("APPROX POSITION XYZ") == 60)
		{
			//���ַ����л�ȡX
			ohead.apX = std::stod(buf.substr(2, 14));
			//���ַ����л�ȡY
			ohead.apY = std::stod(buf.substr(16, 28));
			//���ַ����л�ȡZ
			ohead.apZ = std::stod(buf.substr(29, 42));
			//�����������
			std::cout << "��������:" << ohead.apX << "   " << ohead.apY << "   " << ohead.apZ << endl;
		}
		//��ȡ�۲�ֵ��Ŀ����������
		if (buf.find("# / TYPES OF OBSERV") == 60)
		{
			//��ȡ�۲�ֵ��Ŀ
			ohead.obstypenum = std::stod(buf.substr(5, 6));
			string buf1 = buf.substr(10, 50);
			string tembuf = filereader.trim(buf1);
			ohead.obstype = tembuf;
			std::cout << "�۲�������Ŀ��" << ohead.obstypenum << endl << "�۲�ֵ���ͣ�" << ohead.obstype << endl;
		}
		//��ȡ��Ԫ���
		if (buf.find("INTERVAL") == 60)
		{
			//��ȡ�۲���Ԫ���
			ohead.interval = std::stod(buf.substr(4, 11));
			std::cout << "�۲���Ԫ���:" << ohead.interval << endl;
		}

		//��ȡ�۲�ʱ����ʱ��ϵͳ
		if (buf.find("TIME OF FIRST OBS") == 60)
		{
			//��ȡʱ�������յ�
			ohead.f_y = std::stod(buf.substr(2, 6));
			ohead.f_m = std::stod(buf.substr(10, 12));
			ohead.f_d = std::stod(buf.substr(17, 19));
			ohead.f_h = std::stod(buf.substr(24, 25));
			ohead.f_min = std::stod(buf.substr(30, 31));
			ohead.f_sec = std::stod(buf.substr(34, 43));
			//��ʱ��ϵͳд��char
			for (int i = 0; i < 5; i++)
			{
				ohead.tsys[i] = buf[i + 48];
			}
			std::cout << "�۲�ʱ�̣�" << ohead.f_y << " " << ohead.f_m << " " << ohead.f_d << " " << ohead.f_h << " " << ohead.f_min << " " << ohead.f_sec << endl;
			std::cout << "ʱ��ϵͳ��";
			//���ʱ��ϵͳ
			for (int i = 0; i < 5; i++)
			{
				std::cout << ohead.tsys[i];
			}
			std::cout << endl;
		}
	}

	//��ȡ�۲�վ������������Щ�۲�վ
	//����һ�п�ʼ��ȡ�۲�ֵ�ļ���
	int start = allHangNum+1;
	//��ȡ��25�е�����
	string buf1 = filereader.getLine(broadcastFile, start);
	//�˴�����Ҫ�Ż������Ϊ��λ���أ�
	oepoch.sat_num = std::atoi(buf1.substr(30, 32).c_str());
	std::cout << "������Ŀ��" << oepoch.sat_num << endl;


	//��ȡ�۲�ʱ��
	oepoch.y= std::atoi(buf1.substr(1, 3).c_str());
	oepoch.m= std::atoi(buf1.substr(4, 6).c_str());
	oepoch.d= std::atoi(buf1.substr(8, 9).c_str());
	string strh = buf1.substr(10, 13);
	oepoch.h= std::atoi(filereader.trim(strh).c_str());
	string strm = buf1.substr(13, 16);
	oepoch.min = std::atoi(filereader.trim(strm).c_str());
	string strSec = buf1.substr(16, 10);
	oepoch.sec = std::stod(filereader.trim(strSec).c_str());
	//������Ŀռ���У�
	int ln;
	if((oepoch.sat_num % 12)==0)
	{
		ln = oepoch.sat_num / 12;
	}
	else
	{
		ln = (oepoch.sat_num - (oepoch.sat_num % 12)) / 12 + 1;
	}

	//����վ���ַ���
	string str = "";
	for (int i = 25; i < 25 + ln; i++) {
		string tembufer = filereader.getLine(broadcastFile, i);
		str = str + tembufer.substr(32, 69);
	}
	oepoch.satName = str;
	std::cout << "����վ�ַ���:" << oepoch.satName << endl;

	//�����Ĺ۲�ֵ���ݵĶ�ȡ
	//for (int i = 0; i < oepoch.sat_num; i++)
	for (int i = 0; i <25; i++)
	{
		int x = 25 + ln + i * 2;
		int y = 25 + ln + 1 + i * 2;
		string obs1Line = filereader.getLine(broadcastFile,x);
		string obs1 = obs1Line.substr(1, 13);
		//��ʾ�ӵ�17����ʼ����������14��
		string obs2 = obs1Line.substr(17, 14);
		string obs3 = obs1Line.substr(33, 14);
		string obs4 = obs1Line.substr(48, 16);
		string obs5 = obs1Line.substr(65, 12);

		//���������ȫ�����ǿո��ַ�������Ҫȫ���Ƴ��ո�
		string removeObs1 = filereader.trim(obs1);
		//�Ƴ��ո�֮����Ҫ���������Ϊ��10000000��
		removeObs1 = filereader.trim(removeObs1);
		//Ȼ�����ɵ�����д���ά����
		filereader.observation[i][0] = std::stod(removeObs1);

		string removeObs2 = filereader.trim(obs2);
		removeObs2 = filereader.trim(removeObs2);
		filereader.observation[i][1] = std::stod(removeObs2);


		string removeObs3 = filereader.trim(obs3);
		removeObs3 = filereader.trim(removeObs3);
		filereader.observation[i][2] = std::stod(removeObs3);


		string removeObs4 = filereader.trim(obs4);
		removeObs4 = filereader.trim(removeObs4);
		filereader.observation[i][3] = std::stod(removeObs4.c_str());


		string removeObs5 = filereader.trim(obs5);
		removeObs5 = filereader.trim(removeObs5);
		filereader.observation[i][4] = std::stod(removeObs5);

		string obs2Line = filereader.ReadLine(filereader.filepath, y);

		//����ط��ǳ��¹ؼ�
		//ΪʲôҪ��������
		if (obs2Line.size() < 31)
		{
				string str2(20, ' ');
				obs2Line.append(str2);
		}
		if (obs2Line.size() < 21)
		{
			string str2(30, ' ');
			obs2Line.append(str2);
		}

		string obs6 = obs2Line.substr(1, 14);
		string obs7 = obs2Line.substr(22, 9);
		string obs8 = obs2Line.substr(40, 6);

		string removeObs6 = filereader.trim(obs6);
		removeObs6 = filereader.trim(removeObs6);
		filereader.observation[i][5] = std::stod(removeObs6);

	
		string removeObs7 = filereader.trim(obs7);
		removeObs7 = filereader.trim(removeObs7);
		filereader.observation[i][6] = std::stod(removeObs7);

		
		string removeObs8 = filereader.trim(obs8);
		removeObs8 = filereader.trim(removeObs8);
		filereader.observation[i][7] = std::stod(removeObs8);

	}
	//�����۲�ֵ�����
	//for (int i = 0; i < oepoch.sat_num; i++)
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			std::cout << filereader.observation[i][j]<<"  ";
		}
		std::cout << endl;
	}
}
//N�ļ��Ķ�ȡ
void fileReader::readFile_N(std::vector<navi_body>& nbody, string navigationFile, fileReader& filereader,int allHangNum_n)
{
	

	for (int m = 0; m < nbody.size(); m++)
	{
		int start = allHangNum_n+m*8;
		string buf1 = filereader.getLine(navigationFile, start);
		//�˴�����Ҫ�Ż������Ϊ��λ���أ�


		//��ȡPRN��
		string strPRN = buf1.substr(0, 3);
		nbody[m].PRN = strPRN;
		std::cout << "����PRN�ţ�" << nbody[m].PRN << endl;

		//��ȡʱ����
		string strTOC_year = buf1.substr(4, 4);
		nbody[m].TOC_year = std::atoi(strTOC_year.c_str());
		std::cout << "�꣺" << nbody[m].TOC_year << endl;

		//��ȡʱ����
		string strTOC_month = buf1.substr(9, 2);
		nbody[m].TOC_month = std::atoi(strTOC_month.c_str());
		std::cout << "�£�" << nbody[m].TOC_month << endl;

		//��ȡʱ����
		string strTOC_day = buf1.substr(12, 2);
		nbody[m].TOC_day = std::atoi(strTOC_day.c_str());
		std::cout << "�գ�" << nbody[m].TOC_day << endl;

		//��ȡʱ��ʱ
		string strTOC_hour = buf1.substr(15, 2);
		nbody[m].TOC_hour = std::atoi(strTOC_hour.c_str());
		std::cout << "ʱ��" << nbody[m].TOC_hour << endl;

		//��ȡʱ���
		string strTOC_minute = buf1.substr(18, 2);
		nbody[m].TOC_minute = std::atoi(strTOC_minute.c_str());
		std::cout << "�֣�" << nbody[m].TOC_minute << endl;

		//��ȡʱ����
		string strTOC_second = buf1.substr(21, 2);
		nbody[m].TOC_second = std::atoi(strTOC_second.c_str());
		std::cout << "�룺" << std::fixed << std::setprecision(1) << nbody[m].TOC_second << endl;

		string bufx = buf1;
		//TOCʱ��Ļ�ȡ
		for (int i = 0; i < 3; i++)
		{

			int left;
			int right;
			left = 23 + i * 19;
			right = 23 + (i + 1) * 19;

			//��ȡ����
			string DI = bufx.substr(left, 15);
			string DI_remove;
			if (bufx.substr(left, 1) == "-")
			{
				 DI_remove = DI;
			}
			else {
				 DI_remove = bufx.substr(left+1, 14);
			}
			//����ת����
			double numDI;
			numDI = std::stod(DI_remove);

			//��ȡָ��
			string ZHI = bufx.substr(right - 3, 3);
			string ZHI_remove = filereader.trim(ZHI);

			//ָ��ת����
			double numZHI;
			numZHI = std::stod(ZHI_remove);

			//����ʵ������
			nbody[m].TOC[i] = numDI * pow(10, numZHI);
		}


		//��ȡ���ɲ���6��4�е�����
		for (int i = 0; i < 6; i++) {
			int readStart = start + i + 1;
			string bufx= filereader.getLine(navigationFile, readStart);
			for (int j = 0; j < 4; j++) {
				int left;
				int right;
				left = 4 + j * 19;
				right = 4 + (j + 1) * 19;

				//��ȡ����
				string DI = bufx.substr(left, 15);
				string DI_remove;
				if (bufx.substr(left, 1) == "-")
				{
					DI_remove = DI;
				}
				else {
					DI_remove = bufx.substr(left + 1, 14);
				}
				//����ת����
				double numDI;
				numDI = std::stod(DI_remove);
				//��ȡָ��
				string ZHI = bufx.substr(right - 3, 3);
				string ZHI_remove = filereader.trim(ZHI);

				//ָ��ת����
				double numZHI;
				numZHI = std::stod(ZHI_remove);

				nbody[m].satliteData[i][j] = numDI * pow(10, numZHI);
			}
		}
	}
			//����������йص籨�ķ���ʱ����������������
			//����������ö��ˣ��о�û�ã����Ҹо����õ�ʱ���ٶ�
}
int fileReader::getHeaderNum(string navigationFile)
{
	//��ȡ����
	string fileName = navigationFile;

	/*�����ļ�������*/
	fstream file_n;

	//�����ļ�
	file_n.open(fileName);

	//�ж��Ƿ��ҵ��ļ�
	if (!file_n)
	{
		std::cout << "�ļ���ַ����������Ѱ���ļ�" << endl;
	}
	//��ʱ�洢���ݵ�ÿһ�У������ַ����飬��Ȼ���治�ö���
	string buf;
	//�ļ�������
	int allHangNum_n = 0;
	//whileѭ����Ҫ��ͷ�����ݵĶ�ȡ
	while (1)
	{
		//��ȡ���ļ���һ��
		getline(file_n, buf);
		//��ȡ��һ������֮�󣬿���������������¼һ��
		allHangNum_n++;
		//����ҵ���"END OF HEADER"������ѭ��,�������еļ�¼
		if (buf.find("END OF HEADER") != std::string::npos)
			break;
	}

	return allHangNum_n;
}
string fileReader::getLine(const std::string& fileName, int lineNumber)
{
	std::ifstream file(fileName);
	std::string line;
	int currentLine = 0;

	while (std::getline(file, line)) {
		++currentLine;
		if (currentLine == lineNumber) {
			file.close();
			return line;
		}
	}

	file.close();
	return "";
}
std::vector<caculatePosition> fileReader::nbodyTocacupos(std::vector<navi_body>& nbody, std::vector<caculatePosition>& ca)
{
	for(int k=0;k<ca.size();k++)
	{ 
	ca[k].PRN = nbody[k].PRN;

	for (int i = 0; i < 3; i++) {
		ca[k].TOC[i] = nbody[k].TOC[i];
	}

	for (int i = 0; i < 6; i++) {
		for (int j =0 ; j < 4; j++) {
			ca[k].satliteData[i][j] = nbody[k].satliteData[i][j];
		}
	}

	for (int i = 0; i < 2; i++) {
		ca[k].satliteDataElse[i] = nbody[k].satliteDataElse[i];
	}

	ca[k].TOC_year = nbody[k].TOC_year;
	ca[k].TOC_month = nbody[k].TOC_month;
	ca[k].TOC_day = nbody[k].TOC_day;
	ca[k].TOC_hour = nbody[k].TOC_hour;
	ca[k].TOC_minute = nbody[k].TOC_minute;
	ca[k].TOC_second = nbody[k].TOC_second;
	}
	return ca;
}
void fileReader::nbodyTonbody(struct navi_body& nbody1, struct navi_body& nbody2) {

	nbody1.PRN = nbody2.PRN;

	for (int i = 0; i < 3; i++) {
		nbody1.TOC[i] = nbody2.TOC[i];
	}

	for (int i = 0; i < 6; i++) {
		for (int j =0 ; j < 4; j++) {
			nbody1.satliteData[i][j] = nbody2.satliteData[i][j];
		}
	}

	for (int i = 0; i < 2; i++) {
		nbody1.satliteDataElse[i] = nbody2.satliteDataElse[i];
	}

	nbody1.TOC_year = nbody2.TOC_year;
	nbody1.TOC_month = nbody2.TOC_month;
	nbody1.TOC_day = nbody2.TOC_day;
	nbody1.TOC_hour = nbody2.TOC_hour;
	nbody1.TOC_minute = nbody2.TOC_minute;
	nbody1.TOC_second = nbody2.TOC_second;
}
//��ȡPRN
void fileReader::getPRN(obs_epoch& oepoch, int num, std::vector<int>& vec)
{
	for (int i = 0; i < num; i++) {
		string sub = oepoch.satName.substr(3*i + 1, 2);
		vec.push_back(std::stoi(sub));// �洢ָ����
	}
}

double fileReader::getobsL1(fileReader fr, int col)
{
	return fr.observation[col][2];
}

void fileReader::setobsL1(fileReader& fr, int col, double N)
{
	fr.observation[col][2] = N;
}

//����Ӧ��PRN���������ʹ洢�������������������
void fileReader::getSatliteType(obs_epoch& oepoch, int num, std::vector<char>& vec)
{
	const char* cstr = oepoch.satName.c_str();
	for (int i = 0; i < num; i++) {
		vec.push_back(cstr[3 * i]);
	}
}
//��ȡ��������
int fileReader::getInitPosition(string FileName)
{
	//��ȡ����
	string fileName = FileName;

	/*�����ļ�������*/
	fstream file;

	//�����ļ�
	file.open(fileName);

	//�ж��Ƿ��ҵ��ļ�
	if (!file)
	{
		std::cout << "�ļ���ַ����������Ѱ���ļ�" << endl;
	}

	//��ʱ�洢���ݵ�ÿһ�У������ַ����飬��Ȼ���治�ö���
	string buf;
	return 0;
}

//ȥ��string�е��ַ���
string fileReader::trim(string& s)
{
	int index= 0;
	if (!s.empty())
	{
		while ((index = s.find(' ', index)) != string::npos)
		{
			s.erase(index, 1);
		}
	}
	if (s.empty())
	{
		s = "10000000";
	}
	return s;
}

//��ȡ�ض��еĺ���ʵ��
int fileReader::CountLines(string filename)
{
	ifstream ReadFile;
	int n = 0;
	string tmp;
	ReadFile.open(filename.c_str());//ios::in ��ʾ��ֻ���ķ�ʽ��ȡ�ļ�
	if (ReadFile.fail())//�ļ���ʧ��:����0
	{
		return 0;
	}
	else//�ļ�����
	{
		while (getline(ReadFile, tmp, '\n'))
		{
			n++;
		}
		ReadFile.close();
		return n;
	}
}
string fileReader::ReadLine(string filename, int line)
{
	int lines, i = 0;
	string temp;
	fstream file;
	file.open(filename.c_str());
	lines = CountLines(filename);

	if (line <= 0)
	{
		return "Error 1: �������󣬲���Ϊ0������";
	}
	if (file.fail())
	{
		return "Error 2: �ļ������ڡ�";
	}
	if (line > lines)
	{
		return "Error 3: ���������ļ����ȡ�";
	}
	while (getline(file, temp) && i < line - 1)
	{
		i++;
	}
	file.close();
	return temp;
}
