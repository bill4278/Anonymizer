#include "compressAndUncompress.h"


QString CZlib::str2qstr(const std::string str)
{
	return QString::fromLocal8Bit((str.data()));
}

std::string CZlib::qstr2str(const QString qstr)
{
	QByteArray cdata = qstr.toLocal8Bit();
	return std::string(cdata);
}


/* https://segmentfault.com/a/1190000007301597 */
int CZlib::UnCompress(const char * srcName, const char * tgtName)
{
	
	unz_file_info64 FileInfo;
	unzFile zFile = unzOpen64(srcName);
	if (NULL == zFile)
	{
		std::cout << "empty file: " << srcName << std::endl;
		return -1;
	}

	unz_global_info64 gi;

	if (unzGetGlobalInfo64(zFile, &gi) != UNZ_OK)
	{
		std::cout << "get global info error!!!" << std::endl;
		return -1;
	}
	
	int result;
	std::string tempPath;
	QDir dir;
	for (int i = 0; i < gi.number_entry; i++)
	{
		char file[102400] = { 0 };
		char ext[102400] = { 0 };
		char com[102400] = { 0 };

		if (unzGetCurrentFileInfo64(zFile, &FileInfo, file, sizeof(file), ext, 256, com, 1024) != UNZ_OK)
		{
			std::cout << "error !!!" << std::endl;
		}

		
		
		if ((FileInfo.external_fa & FILE_ATTRIBUTE_DIRECTORY))
		{
			QDir dir;
			tempPath = qstr2str(str2qstr(tgtName) + file);
			dir.mkdir(str2qstr(tempPath));
		}
		
		result = unzOpenCurrentFile(zFile);
		char data[102400] = { 0 };
		int size;
		QString path = str2qstr(tgtName) + file;
		QFile f(path);
		std::cout << qstr2str(path) << std::endl;
		f.open(QFile::WriteOnly);
		while (true)
		{
			size = unzReadCurrentFile(zFile, data, sizeof(data));
			if (size <= 0)
			{
				break;
			}
			f.write(data, size);
		}
		f.close();
		unzCloseCurrentFile(zFile);
		if (i < gi.number_entry - 1 && unzGoToNextFile(zFile) != UNZ_OK)
		{
			std::cout << "error2 !!!" << std::endl;
		}
	}

	unzClose(zFile);
	return 1;

}

int CZlib::Compress(QFileInfoList srcFileList, const char * tgtName, const char * uncompressPath_char)
{

	
	zipFile zFile = zipOpen64(tgtName, 0);
	if (zFile == NULL)
	{
		std::cout << "failed to create zip file" << std::endl;
		return -1;
	}
	for (int i = 0; i != srcFileList.size(); i++)
	{
		//初始化写入zip的文件信息
		zip_fileinfo zi;
		zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
		zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
		zi.dosDate = 0;
		zi.internal_fa = 0;
		zi.external_fa = 0;
		
		std::string filePath_str = srcFileList.at(i).canonicalFilePath().toStdString();
		
		std::string newFilePath_str = filePath_str;

		const char *filePath = filePath_str.c_str();
		
		newFilePath_str.replace(0, strlen(uncompressPath_char), std::string(""));

		zipOpenNewFileInZip64(zFile, newFilePath_str.c_str(),&zi, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION,1);
		FILE* file = fopen(filePath, "rb");
		if (file == NULL)
		{
			std::cout << "failed to open file : " << filePath_str << std::endl;
			zipCloseFileInZip(zFile); //关闭zip文件
			return -1;
		}
		//读入源文件并写入zip文件
		char buf[100 * 1024]; //buffer
		int numBytes = 0;
		while (!feof(file))
		{
			numBytes = fread(buf, 1, sizeof(buf), file);
			zipWriteInFileInZip(zFile, buf, numBytes);
			if (ferror(file))
				break;
		}
		fclose(file);
	}
	//关闭zip文件
	zipCloseFileInZip(zFile);
	zipClose(zFile, NULL);
	return 1;
}