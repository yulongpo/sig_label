#pragma once
//#include <QVector>
//#include <cassert>

#ifdef _DEBUG
#define PRINT   printf
#else
#define PRINT(...)
#endif

struct barInfo {
	double midx;
	double width;
	double ymin;
	double ymax;
};

