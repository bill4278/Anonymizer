#pragma once
#pragma comment(lib,"rpcrt4.lib")

#pragma comment(lib,"WS2_32.lib")

#include <QDir>
#include <QFile>

class commonOperation : public QObject
{
	Q_OBJECT
public:
	int removeFile(const char * filePath);
	int renameFile(const char * oldFilePath, const char * newFileName);

	QString str2qstr(const std::string str);
	std::string qstr2str(const QString qstr);
	QFileInfoList getFileList(QString folderChoose, QStringList nameFilters);
};