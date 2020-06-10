#pragma once
#pragma comment(lib,"rpcrt4.lib")

#pragma comment(lib,"WS2_32.lib")

#include <QObject>
#include <QFile>
#include <QDir>
#include <iostream>

#include "Thread_DCMTK_anonymizeDCM.h"
#include "commonOperation.h"

class threadAnonyNoSuffix :public QObject
{
	Q_OBJECT
public:
	threadAnonyNoSuffix(QObject* parent = NULL);
private:
	commonOperation m_coP;
	QStringList nameFiltersDcm;
	QStringList nameFiltersAllFile;
	QStringList nameFiltersAllSuffix;
	threadAnonyDCM m_anonyDcm;
	bool dcmProgressFinished = false;
	public slots:
	void anonymizeNosuffix(const QString folderChoose);
	void getProgressValue(const float value, const int bar_No);
	void getProgressError(QString Qstr);
	void getProgressStatus(QString Qstr);
	void getFatalError(QString Qstr);
	void getProgressFinished(bool isFinished);
signals:
	void progressValue(const float value, const int bar_No);
	void progressError(QString Qstr);
	void progressStatus(QString Qstr);
	void fatalError(QString Qstr);//严重错误,请求中止.
	void progressFinished(bool isFinish, int process_No);
};
