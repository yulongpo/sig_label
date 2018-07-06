#include "sig.h"
//#include <QMap>
#include <iterator>

Sig::Sig(std::string filePath) :
	filePath(filePath)
{
	openFile(filePath);
}


void Sig::openFile(std::string filePath)
{
	sigStream.close();
	Sig::filePath = filePath;
	sigStream.open(filePath, std::ios::binary);
	if (!sigStream) {
		std::cout << "STREAM NOT OK!" << std::endl;
		state = false;
		allFrameNums = 0;
		return;
	}

	std::cout << "STREAM OK!" << std::endl;
	sigStream.read(reinterpret_cast<char*>(&waveLen), sizeof(int));
	if (waveLen != 50576) {
		state = false;
		std::cout << "WAVELEN: " << waveLen << std::endl;
		return;
	}

	std::cout << "�򿪳ɹ���" << std::endl;
	sigStream.seekg(0, SEEK_END);
	end = sigStream.tellg();
	sigStream.seekg(0);

	while (sigStream.tellg() < end) {
		sigPos.push_back(sigStream.tellg());
		sigStream.read(reinterpret_cast<char*>(&waveLen), sizeof(int));
		sigStream.read(reinterpret_cast<char*>(wave), 50576 * sizeof(char));
		sigStream.read(reinterpret_cast<char*>(&resLen), sizeof(unsigned int));
		const size_t n = resLen / sizeof(WaveResult);
		WaveResult *waveRes = new WaveResult[n];
		sigStream.read(reinterpret_cast<char*>(waveRes), resLen);
		delete waveRes;
	}
	allFrameNums = unsigned long(sigPos.size());
	state = true;
	//// �жϵ�ǰ֡λ��
	//if (curFrameInfo.curFrameNum < 1)
	//	curFrameInfo.curFrameNum = 1;
	//else if (curFrameInfo.curFrameNum > allFrameNums)
	//	curFrameInfo.curFrameNum = allFrameNums;

	//sigStream.seekg(sigPos[curFrameInfo.curFrameNum - 1]);

	curFrameInfo.curFrameNum = 1; //��ȡ���ļ�����ʾ��һ֡
	readFrame(curFrameInfo);

}

void Sig::readFrame(sigFrame &frame)
{
	frame.result.clear();

	sigStream.seekg(sigPos[frame.curFrameNum - 1]);
	sigStream.read(reinterpret_cast<char*>(&frame.spectrumLength), sizeof(int));
	sigStream.read(reinterpret_cast<char*>(frame.aveSpectrum), 25288 * sizeof(char));
	sigStream.read(reinterpret_cast<char*>(frame.maxSpectrum), 25288 * sizeof(char));
	sigStream.read(reinterpret_cast<char*>(&frame.resNums), sizeof(unsigned int));

	const size_t n = frame.resNums / sizeof(WaveResult);
	WaveResult *waveRes = new WaveResult[n];
	sigStream.read(reinterpret_cast<char*>(waveRes), frame.resNums);

	for (int i = 0; i < n; ++i) {
		frame.result.push_back(waveRes[i]);
	}
	delete waveRes;

}

