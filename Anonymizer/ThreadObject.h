#pragma once
#pragma comment(lib,"rpcrt4.lib")

#pragma comment(lib,"WS2_32.lib")

#include "ui_Anonymizer.h"
#include "compressAndUncompress.h"
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QCloseEvent>
#include <QObject>

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmjpeg/djdecode.h"
#include "dcmtk/dcmdata/dcrledrg.h"

class ThreadFunction : public QObject
{
	Q_OBJECT
public:
	explicit ThreadFunction(QObject *parent = nullptr);
	Ui::mainWindow ui;
	
public slots:

	void slot_btn_chooseFolder();
	void slot_btn_chooseFolderForDcm();
	void slot_btn_chooseFolderForZip();
	void slot_btn_chooseFolderForNoSuffix();
	//void slot_btn_collpaseLogBrowser();

	//void closeEvent(QCloseEvent *event);

	void anonymizeZip(QString folderChoose, QFileInfoList zipList);
	void DCMTK_anonymizeDcm(QString folderChoose, QFileInfoList dcmList);
	void anonymizeNoSuffix(QString folderChoose);
	QFileInfoList getFileList(QString folderChoose, QStringList nameFilters);
	void removeFile(const char * filePath);
	void renameFile(const char * oldFilePath, const char * newFileName);
	//bool is_logBrowserCollpased = true;
	void printLog(QString logQStr);
	void printError(QString errQStr);

private:
	QFileDialog *fileDialog = new QFileDialog();
	QStringList nameFiltersDcm;
	QStringList nameFiltersZip;
	QStringList nameFiltersAllSuffix;
	QStringList nameFiltersAllFile;
	CZlib mZlib;
};

