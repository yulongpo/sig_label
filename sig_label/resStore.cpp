#include "resStore.h"

resStore::resStore(QString fileName)
	: _fileName(fileName)
{

}

QString resStore::fileName()
{
	return _fileName;
}

void resStore::setFileName(QString fileName)
{
	_fileName = fileName;
}
