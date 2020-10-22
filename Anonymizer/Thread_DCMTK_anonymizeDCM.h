#pragma once
#pragma comment(lib,"rpcrt4.lib")

#pragma comment(lib,"WS2_32.lib")

#include "io.h"
#include <QObject>
#include <QFile>
#include <QDir>
#include <iostream>
#include "commonOperation.h"
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmjpeg/djdecode.h"
#include "dcmtk/dcmdata/dcrledrg.h"


//https://blog.csdn.net/u012372584/article/details/80509217
//https://blog.csdn.net/zhenguo26/article/details/82588415
//https://blog.csdn.net/apple_2333/article/details/86756476
//https://blog.csdn.net/zong596568821xp/article/details/79011150

class threadAnonyDCM :public QObject
{
	Q_OBJECT
public:
	threadAnonyDCM(QObject* parent = NULL);
private:
	commonOperation m_coP;
	QStringList nameFiltersDcm;
public slots:
	void DCMTK_anonymizeDcm_2(const QString folderChoose, const QFileInfoList dcmList);
	void DCMTK_anonymizeDcm(const QString folderChoose);
signals:
	void progressValue(const float value, const int bar_No);
	void progressError(QString Qstr);
	void progressStatus(QString Qstr);
	void fatalError(QString Qstr);//严重错误,请求中止.
	void progressFinished(bool isFinish, int process_No);
};