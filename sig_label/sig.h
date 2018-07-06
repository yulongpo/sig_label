#pragma once
#include <iostream>
#include <fstream>
//#include <QString>
#include <string>
#include <vector>
#include "myUtil.h"
#include <QMap>

const unsigned int WAVELENS = 25288;


struct WaveResult
{
	char  sWaveID[36];       //�ز�ID
	double dFreqMid;          //�ز�����Ƶ��,��λMHz
	double dBW;               //�ز�����,��λMHz
	float dSignalLevel;      //�źŵ�ƽ,��λdBm
	float dCN;               //�����,��λdB
};

struct labelResult {
	unsigned int start;
	unsigned int end;
};


struct sigFrame {
	unsigned long curFrameNum;				// ֻ�洢��ǰ֡�Ľ��

	int spectrumLength;					// Ƶ�׳��ȣ�ӦΪ��50576
	char aveSpectrum[25288];	// ƽ��������
	char maxSpectrum[25288];	// ��󱣳�������
	unsigned int resNums;				// ��֡��⵽���ز�����
	std::vector<WaveResult> result;		// ��֡������ز������
};


class Sig {
public:
	Sig() {};
	Sig(std::string filePath);
	~Sig() {
		sigStream.close();
	};

	bool state = false;

	std::ifstream sigStream;
	//std::ofstream outStream;


	unsigned long allFrameNums = 0;
	//unsigned long curFrameNum = 1;
	//WaveResult result;
	sigFrame curFrameInfo;

	void openFile(std::string filePath);
	void readFrame(sigFrame &frame);


private:
	std::string filePath;
	std::vector<std::ifstream::pos_type> sigPos;
	std::ifstream::pos_type end;

	int waveLen;
	unsigned int resLen;
	char wave[50576];

};
