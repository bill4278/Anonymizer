#include <iostream>
#include "commonOperation.h"
#include <errno.h>

QString commonOperation::str2qstr(const std::string str)
{
	return QString::fromLocal8Bit((str.data()));
}

std::string commonOperation::qstr2str(const QString qstr)
{
	QByteArray cdata = qstr.toLocal8Bit();
	return std::string(cdata);
}

int commonOperation::removeFile(const char * filePath)
{
	if (std::remove(filePath) == -1)
	{
		std::cout << "Error: " << strerror(errno) << std::endl;
		return -1;
	}
	return 1;
}

int commonOperation::renameFile(const char * oldFilePath, const char * newFileName)
{
	if (std::rename(oldFilePath, newFileName) == -1)
	{
		std::cout << "Error: " << strerror(errno) << std::endl;
		return -1;
	}
	return 1;
}

QFileInfoList commonOperation::getFileList(QString folderChoose, QStringList nameFilters)
{
	QString message;

	QDir dir(folderChoose);
	QFileInfoList file_list = dir.entryInfoList(nameFilters, QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

	for (int i = 0; i != folder_list.size(); i++)
	{
		QString filePath = folder_list.at(i).absoluteFilePath();
		QFileInfoList child_file_list = getFileList(filePath, nameFilters);
		file_list.append(child_file_list);
	}

	return file_list;

}