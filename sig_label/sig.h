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
	char  sWaveID[36];       //载波ID
	double dFreqMid;          //载波中心频点,单位MHz
	double dBW;               //载波带宽,单位MHz
	float dSignalLevel;      //信号电平,单位dBm
	float dCN;               //载噪比,单位dB
};

struct labelResult {
	unsigned int start;
	unsigned int end;
};


struct sigFrame {
	unsigned long curFrameNum;				// 只存储当前帧的结果

	int spectrumLength;					// 频谱长度，应为：50576
	char aveSpectrum[25288];	// 平均谱数据
	char maxSpectrum[25288];	// 最大保持谱数据
	unsigned int resNums;				// 该帧检测到的载波数量
	std::vector<WaveResult> result;		// 该帧读入的载波检测结果
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
