#pragma once

#include "stdio.h"
#include "stdlib.h"
#include "zlib.h"
#include "unzip.h"
#include "zip.h"
#include <iostream>
#include <QFile>
#include <QDir>
#include "commonOperation.h"

#define FILE_ATTRIBUTE_DIRECTORY            0x00000010

class CZlib
{
public:

	int Compress(QFileInfoList srcFileList, const char * tgtName, const char * uncompressPath_char);
	int UnCompress(const char * srcName, const char * tgtName);

private:

	commonOperation m_coP;

};