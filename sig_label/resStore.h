#pragma once

#include <fstream>
#include <QtCore\QString>

#include "myUtil.h"

class resStore {
public:
	resStore() {};
	resStore(QString fileName);
	virtual ~resStore() {};

	void read();
	void write();

	QString fileName();
	void setFileName(QString fileName);

private:
	QString _fileName;



};
