#pragma once

#include "stdio.h"
#include "stdlib.h"
#include "zlib.h"
#include "unzip.h"
#include "zip.h"
#include <iostream>
#include <QFile>
#include <QDir>

#define FILE_ATTRIBUTE_DIRECTORY            0x00000010

class CZlib
{
public:
	//CZlib();
	//~CZlib();
	int Compress(QFileInfoList srcFileList, const char * tgtName, const char * uncompressPath_char);
	int UnCompress(const char * srcName, const char * tgtName);
	QString str2qstr(const std::string str);
	std::string qstr2str(const QString qstr);
private:

};