#include "FlieCenter.h"
#include <iostream>
#include <fstream>
using namespace std;
int openFile(char path) {
    int x, sum = 0;
    ifstream srcFile("path", ios::in); //���ı�ģʽ��in.txt����
    if (!srcFile) { //��ʧ��
        cout << "error opening source file." << endl;
        return 0;
    }
    ofstream destFile("out.txt", ios::out); //���ı�ģʽ��out.txt��д
    if (!destFile) {
        srcFile.close(); //�������ǰ�������ǹر���ǰ�򿪹����ļ�
        cout << "error opening destination file." << endl;
        return 0;
    }
    //��������cin������ifstream����
    while (srcFile >> x) {
        sum += x;
        //������ cout ����ʹ�� ofstream ����
        destFile << x << " ";
    }
    cout << "sum��" << sum << endl;
    destFile.close();
    srcFile.close();
    return 0;
}